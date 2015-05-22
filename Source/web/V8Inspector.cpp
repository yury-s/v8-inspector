// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"
#include "web/V8Inspector.h"

#include "bindings/core/v8/MainThreadDebugger.h"
#include "bindings/core/v8/V8PerIsolateData.h"
#include "core/InspectorBackendDispatcher.h"
#include "core/InspectorFrontend.h"
#include "core/inspector/AsyncCallTracker.h"
#include "core/inspector/InjectedScriptHost.h"
#include "core/inspector/InjectedScriptManager.h"
#include "core/inspector/InspectorDebuggerAgent.h"
#include "core/inspector/InspectorState.h"
#include "core/inspector/InstrumentingAgents.h"
#include "core/inspector/PageDebuggerAgent.h"
#include "core/inspector/PageRuntimeAgent.h"
#include "platform/JSONValues.h"
#include "public/platform/Platform.h"
#include "web/V8InspectorClient.h"
#include "wtf/Noncopyable.h"
#include "wtf/text/WTFString.h"

namespace blink {

class ClientMessageLoopAdapter : public MainThreadDebugger::ClientMessageLoop {
public:
    ~ClientMessageLoopAdapter() override
    {
        s_instance = nullptr;
    }

    static void ensureMainThreadDebuggerCreated(V8InspectorClient* client)
    {
        if (s_instance)
            return;
        OwnPtr<ClientMessageLoopAdapter> instance = adoptPtr(new ClientMessageLoopAdapter(adoptPtr(client->createClientMessageLoop())));
        s_instance = instance.get();

        v8::Isolate* isolate = V8PerIsolateData::mainThreadIsolate();
        V8PerIsolateData* data = V8PerIsolateData::from(isolate);
        data->setScriptDebugger(MainThreadDebugger::create(instance.release(), isolate));
    }

private:
    ClientMessageLoopAdapter(PassOwnPtr<V8InspectorClient::ClientMessageLoop> messageLoop)
        : m_running(false)
        , m_messageLoop(messageLoop) { }

    void run(LocalFrame* frame) override
    {
        if (m_running)
            return;
        m_running = true;
        m_messageLoop->run();
        m_running = false;
    }

    void quitNow() override
    {
        m_messageLoop->quitNow();
    }

    bool m_running;
    OwnPtr<V8InspectorClient::ClientMessageLoop> m_messageLoop;
    static ClientMessageLoopAdapter* s_instance;
};

ClientMessageLoopAdapter* ClientMessageLoopAdapter::s_instance = nullptr;

class V8InspectorInjectedScriptHostClient: public InjectedScriptHostClient {
public:
    V8InspectorInjectedScriptHostClient() { }

    ~V8InspectorInjectedScriptHostClient() override { }

    void muteWarningsAndDeprecations()
    {
    }

    void unmuteWarningsAndDeprecations()
    {
    }
};

V8Inspector::V8Inspector(V8InspectorClient* client)
    : m_client(client)
    , m_attached(false)
    , m_instrumentingAgents(InstrumentingAgents::create())
    , m_injectedScriptManager(InjectedScriptManager::createForPage())
    , m_state(adoptPtrWillBeNoop(new InspectorCompositeState(this)))
    , m_agents(m_instrumentingAgents.get(), m_state.get())
{
    InjectedScriptManager* injectedScriptManager = m_injectedScriptManager.get();

    ClientMessageLoopAdapter::ensureMainThreadDebuggerCreated(m_client);
    MainThreadDebugger* mainThreadDebugger = MainThreadDebugger::instance();

    OwnPtrWillBeRawPtr<PageRuntimeAgent> pageRuntimeAgentPtr(PageRuntimeAgent::create(injectedScriptManager, this, mainThreadDebugger->debugger(), nullptr));
    PageRuntimeAgent* pageRuntimeAgent = pageRuntimeAgentPtr.get();
    m_agents.append(pageRuntimeAgentPtr.release());

    OwnPtrWillBeRawPtr<InspectorDebuggerAgent> debuggerAgentPtr(PageDebuggerAgent::create(MainThreadDebugger::instance(), nullptr, injectedScriptManager, nullptr, pageRuntimeAgent->debuggerId()));
    InspectorDebuggerAgent* debuggerAgent = debuggerAgentPtr.get();
    m_agents.append(debuggerAgentPtr.release());
    m_asyncCallTracker = adoptPtrWillBeNoop(new AsyncCallTracker(debuggerAgent, m_instrumentingAgents.get()));

    m_injectedScriptManager->injectedScriptHost()->init(
        nullptr,
        debuggerAgent,
        nullptr,
        mainThreadDebugger->debugger(),
        adoptPtr(new V8InspectorInjectedScriptHostClient()));
}

V8Inspector::~V8Inspector()
{
    detach();
    m_injectedScriptManager->disconnect();
    m_agents.discardAgents();
    m_instrumentingAgents->reset();
}

void V8Inspector::attach(const String& hostId)
{
    if (m_attached)
        return;
    // Set the attached bit first so that sync notifications were delivered.
    m_attached = true;
    m_inspectorFrontend = adoptPtr(new InspectorFrontend(this));
    // We can reconnect to existing front-end -> unmute state.
    m_state->unmute();
    m_agents.setFrontend(m_inspectorFrontend.get());

    m_inspectorBackendDispatcher = InspectorBackendDispatcher::create(this);
    m_agents.registerInDispatcher(m_inspectorBackendDispatcher.get());
}

void V8Inspector::reattach(const String& hostId, const String& savedState)
{
    if (m_attached)
        return;

    attach(hostId);
    m_state->loadFromCookie(savedState);
    m_agents.restore();
}

void V8Inspector::detach()
{
    if (!m_attached)
        return;

    m_inspectorBackendDispatcher->clearFrontend();
    m_inspectorBackendDispatcher.clear();

    // Destroying agents would change the state, but we don't want that.
    // Pre-disconnect state will be used to restore inspector agents.
    m_state->mute();
    m_agents.clearFrontend();
    m_inspectorFrontend.clear();

    m_attached = false;
}

void V8Inspector::dispatchOnInspectorBackend(const String& message)
{
    if (!m_attached)
        return;
    if (m_inspectorBackendDispatcher)
        m_inspectorBackendDispatcher->dispatch(message);
}

void V8Inspector::sendProtocolResponse(int callId, PassRefPtr<JSONObject> message)
{
    if (!m_attached)
        return;
    m_client->sendProtocolMessage(callId, message->toJSONString(), m_stateCookie);
    m_stateCookie = String();
}

void V8Inspector::sendProtocolNotification(PassRefPtr<JSONObject> message)
{
    if (!m_attached)
        return;
    m_client->sendProtocolMessage(0, message->toJSONString(), String());
}

void V8Inspector::updateInspectorStateCookie(const String& state)
{
    m_stateCookie = state;
}

} // namespace blink
