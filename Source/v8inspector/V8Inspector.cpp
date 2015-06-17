// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"

#include "v8inspector/V8Inspector.h"

#include "core/InspectorBackendDispatcher.h"
#include "core/InspectorFrontend.h"
#include "core/inspector/InjectedScriptHost.h"
#include "core/inspector/InjectedScriptManager.h"
#include "core/inspector/InspectorFrontendChannel.h"
#include "core/inspector/InspectorState.h"
#include "core/inspector/InspectorStateClient.h"
#include "core/inspector/WorkerDebuggerAgent.h"
#include "core/inspector/WorkerRuntimeAgent.h"
#include "wtf/PassOwnPtr.h"

namespace blink {

namespace {

class ChannelImpl final : public InspectorFrontendChannel {
public:
    explicit ChannelImpl() { }
    virtual ~ChannelImpl() { }
private:
    virtual void sendProtocolResponse(int callId, PassRefPtr<JSONObject> message) override
    {
        printf("ChannelImpl::sendProtocolResponse \n");
//         printf("ChannelImpl::sendProtocolResponse callId = %d message = %s\n", callId, message->toPrettyJSONString().utf8().data());
    }
    virtual void sendProtocolNotification(PassRefPtr<JSONObject> message) override
    {
        printf("ChannelImpl::sendProtocolNotification \n");
//         printf("ChannelImpl::sendProtocolNotification message = %s\n", message->toPrettyJSONString().utf8().data());
    }
    virtual void flush() override { }
};

class StateClientImpl final : public InspectorStateClient {
public:
    StateClientImpl() { }
    virtual ~StateClientImpl() { }

private:
    virtual void updateInspectorStateCookie(const String& cookie) override { }
};

}

class InjectedScriptHostClientImpl: public InjectedScriptHostClient {
public:
    InjectedScriptHostClientImpl() { }

    ~InjectedScriptHostClientImpl() override { }
};

V8Inspector::V8Inspector(v8::Isolate* isolate)
    : m_stateClient(adoptPtr(new StateClientImpl()))
    , m_state(adoptPtrWillBeNoop(new InspectorCompositeState(m_stateClient.get())))
    , m_injectedScriptManager(InjectedScriptManager::createForWorker())
    , m_workerThreadDebugger(WorkerThreadDebugger::create(isolate))
    , m_agents(m_state.get())
    , m_paused(false)
{
    ScriptState* scriptState = ScriptState::current(isolate);

    OwnPtrWillBeRawPtr<WorkerRuntimeAgent> workerRuntimeAgent = WorkerRuntimeAgent::create(m_injectedScriptManager.get(), m_workerThreadDebugger->debugger(), nullptr, this);
    m_workerRuntimeAgent = workerRuntimeAgent.get();
    m_agents.append(workerRuntimeAgent.release());

    OwnPtrWillBeRawPtr<WorkerDebuggerAgent> workerDebuggerAgent = WorkerDebuggerAgent::create(m_workerThreadDebugger.get(), m_injectedScriptManager.get(), scriptState);
    m_workerDebuggerAgent = workerDebuggerAgent.get();
    m_agents.append(workerDebuggerAgent.release());

    m_injectedScriptManager->injectedScriptHost()->init(m_workerDebuggerAgent, nullptr, m_workerThreadDebugger->debugger(), adoptPtr(new InjectedScriptHostClientImpl()));
}

V8Inspector::~V8Inspector()
{
}

void V8Inspector::registerModuleAgent(PassOwnPtrWillBeRawPtr<InspectorAgent> agent)
{
    m_agents.append(agent);
}

void V8Inspector::connectFrontend()
{
    ASSERT(!m_frontend);
    m_state->unmute();
    m_frontendChannel = adoptPtr(new ChannelImpl());
    m_frontend = adoptPtr(new InspectorFrontend(m_frontendChannel.get()));
    m_backendDispatcher = InspectorBackendDispatcher::create(m_frontendChannel.get());
    m_agents.registerInDispatcher(m_backendDispatcher.get());
    m_agents.setFrontend(m_frontend.get());
}

void V8Inspector::disconnectFrontend()
{
    if (!m_frontend)
        return;
    m_backendDispatcher->clearFrontend();
    m_backendDispatcher.clear();
    // Destroying agents would change the state, but we don't want that.
    // Pre-disconnect state will be used to restore inspector agents.
    m_state->mute();
    m_agents.clearFrontend();
    m_frontend.clear();
    m_frontendChannel.clear();
}

void V8Inspector::restoreInspectorStateFromCookie(const String& inspectorCookie)
{
    ASSERT(!m_frontend);
    connectFrontend();
    m_state->loadFromCookie(inspectorCookie);

    m_agents.restore();
}

void V8Inspector::dispatchMessageFromFrontend(const String& message)
{
    if (m_backendDispatcher)
        m_backendDispatcher->dispatch(message);
}

void V8Inspector::dispose()
{
    disconnectFrontend();
}

void V8Inspector::interruptAndDispatchInspectorCommands()
{
    m_workerDebuggerAgent->interruptAndDispatchInspectorCommands();
}

void V8Inspector::resumeStartup()
{
    m_paused = false;
}

bool V8Inspector::isRunRequired()
{
    return m_paused;
}

void V8Inspector::pauseOnStart()
{
    m_paused = true;
    printf("V8Inspector::pauseOnStart\n");
}

} // namespace blink
