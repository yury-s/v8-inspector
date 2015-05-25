// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"

#include "v8inspector/V8Inspector.h"

#include "core/InspectorBackendDispatcher.h"
#include "core/InspectorFrontend.h"
#include "core/inspector/AsyncCallTracker.h"
#include "core/inspector/InjectedScriptHost.h"
#include "core/inspector/InjectedScriptManager.h"
#include "core/inspector/InspectorConsoleAgent.h"
#include "core/inspector/InspectorFrontendChannel.h"
#include "core/inspector/InspectorHeapProfilerAgent.h"
#include "core/inspector/InspectorInstrumentation.h"
#include "core/inspector/InspectorProfilerAgent.h"
#include "core/inspector/InspectorState.h"
#include "core/inspector/InspectorStateClient.h"
#include "core/inspector/InspectorTimelineAgent.h"
#include "core/inspector/InstrumentingAgents.h"
#include "core/inspector/WorkerConsoleAgent.h"
#include "core/inspector/WorkerDebuggerAgent.h"
#include "core/inspector/WorkerRuntimeAgent.h"
#include "core/workers/WorkerGlobalScope.h"
#include "core/workers/WorkerReportingProxy.h"
#include "core/workers/WorkerThread.h"
#include "wtf/PassOwnPtr.h"

namespace blink {

namespace {

class PageInspectorProxy final : public InspectorFrontendChannel {
    WTF_MAKE_FAST_ALLOCATED(PageInspectorProxy);
public:
    explicit PageInspectorProxy(WorkerGlobalScope* workerGlobalScope) : m_workerGlobalScope(workerGlobalScope) { }
    virtual ~PageInspectorProxy() { }
private:
    virtual void sendProtocolResponse(int callId, PassRefPtr<JSONObject> message) override
    {
        // Worker messages are wrapped, no need to handle callId.
        m_workerGlobalScope->thread()->workerReportingProxy().postMessageToPageInspector(message->toJSONString());
    }
    virtual void sendProtocolNotification(PassRefPtr<JSONObject> message) override
    {
        m_workerGlobalScope->thread()->workerReportingProxy().postMessageToPageInspector(message->toJSONString());
    }
    virtual void flush() override { }
    WorkerGlobalScope* m_workerGlobalScope;
};

class WorkerStateClient final : public InspectorStateClient {
    WTF_MAKE_FAST_ALLOCATED(WorkerStateClient);
public:
    WorkerStateClient(WorkerGlobalScope* context) { }
    virtual ~WorkerStateClient() { }

private:
    virtual void updateInspectorStateCookie(const String& cookie) override { }
};

}

class WorkerInjectedScriptHostClient: public InjectedScriptHostClient {
public:
    WorkerInjectedScriptHostClient() { }

    ~WorkerInjectedScriptHostClient() override { }
};

V8Inspector::V8Inspector(WorkerGlobalScope* workerGlobalScope)
    : m_workerGlobalScope(workerGlobalScope)
    , m_stateClient(adoptPtr(new WorkerStateClient(workerGlobalScope)))
    , m_state(adoptPtrWillBeNoop(new InspectorCompositeState(m_stateClient.get())))
    , m_instrumentingAgents(InstrumentingAgents::create())
    , m_injectedScriptManager(InjectedScriptManager::createForWorker())
    , m_workerThreadDebugger(WorkerThreadDebugger::create(workerGlobalScope))
    , m_agents(m_instrumentingAgents.get(), m_state.get())
    , m_paused(false)
{
    OwnPtrWillBeRawPtr<WorkerRuntimeAgent> workerRuntimeAgent = WorkerRuntimeAgent::create(m_injectedScriptManager.get(), m_workerThreadDebugger->debugger(), workerGlobalScope, this);
    m_workerRuntimeAgent = workerRuntimeAgent.get();
    m_agents.append(workerRuntimeAgent.release());

    OwnPtrWillBeRawPtr<WorkerDebuggerAgent> workerDebuggerAgent = WorkerDebuggerAgent::create(m_workerThreadDebugger.get(), workerGlobalScope, m_injectedScriptManager.get());
    m_workerDebuggerAgent = workerDebuggerAgent.get();
    m_agents.append(workerDebuggerAgent.release());
    m_asyncCallTracker = adoptPtrWillBeNoop(new AsyncCallTracker(m_workerDebuggerAgent, m_instrumentingAgents.get()));

    m_agents.append(InspectorProfilerAgent::create(m_injectedScriptManager.get(), 0));
    m_agents.append(InspectorHeapProfilerAgent::create(m_injectedScriptManager.get()));

    OwnPtrWillBeRawPtr<WorkerConsoleAgent> workerConsoleAgent = WorkerConsoleAgent::create(m_injectedScriptManager.get(), workerGlobalScope);
    WorkerConsoleAgent* workerConsoleAgentPtr = workerConsoleAgent.get();
    m_agents.append(workerConsoleAgent.release());

    m_agents.append(InspectorTimelineAgent::create());

    m_injectedScriptManager->injectedScriptHost()->init(workerConsoleAgentPtr, m_workerDebuggerAgent, nullptr, m_workerThreadDebugger->debugger(), adoptPtr(new WorkerInjectedScriptHostClient()));
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
    m_frontendChannel = adoptPtr(new PageInspectorProxy(m_workerGlobalScope));
    m_frontend = adoptPtr(new InspectorFrontend(m_frontendChannel.get()));
    m_backendDispatcher = InspectorBackendDispatcher::create(m_frontendChannel.get());
    m_agents.registerInDispatcher(m_backendDispatcher.get());
    m_agents.setFrontend(m_frontend.get());
    InspectorInstrumentation::frontendCreated();
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
    InspectorInstrumentation::frontendDeleted();
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
    m_instrumentingAgents->reset();
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
    MessageQueueWaitResult result;
    m_workerGlobalScope->thread()->willEnterNestedLoop();
    do {
        result = m_workerGlobalScope->thread()->runDebuggerTask();
    // Keep waiting until execution is resumed.
    } while (result == MessageQueueMessageReceived && m_paused);
    m_workerGlobalScope->thread()->didLeaveNestedLoop();
}

DEFINE_TRACE(V8Inspector)
{
    visitor->trace(m_workerGlobalScope);
    visitor->trace(m_state);
    visitor->trace(m_instrumentingAgents);
    visitor->trace(m_injectedScriptManager);
    visitor->trace(m_workerThreadDebugger);
    visitor->trace(m_backendDispatcher);
    visitor->trace(m_agents);
    visitor->trace(m_workerDebuggerAgent);
    visitor->trace(m_asyncCallTracker);
    visitor->trace(m_workerRuntimeAgent);
}

} // namespace blink
