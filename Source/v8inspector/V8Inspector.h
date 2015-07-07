// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8Inspector_h
#define V8Inspector_h

#include "bindings/core/v8/WorkerThreadDebugger.h"
#include "core/inspector/InspectorBaseAgent.h"
#include "core/inspector/InspectorRuntimeAgent.h"
#include "wtf/Forward.h"
#include "wtf/Noncopyable.h"
#include "wtf/OwnPtr.h"
#include "wtf/RefPtr.h"

namespace blink {

class InjectedScriptManager;
class InspectorBackendDispatcher;
class InspectorFrontend;
class InspectorFrontendChannel;
class InspectorStateClient;
class WorkerDebuggerAgent;
class WorkerRuntimeAgent;
class WorkerThreadDebugger;

class V8Inspector : public InspectorRuntimeAgent::Client {
    WTF_MAKE_NONCOPYABLE(V8Inspector);
public:
    explicit V8Inspector(v8::Isolate*, PassOwnPtr<WorkerThreadDebugger::ClientMessageLoop>);
    ~V8Inspector();
    DECLARE_TRACE();

    void connectFrontend(InspectorFrontendChannel*);
    void disconnectFrontend();
    void restoreInspectorStateFromCookie(const String& inspectorCookie);
    void dispatchMessageFromFrontend(const String&);
    void dispose();

    void pauseOnStart();

private:
    // InspectorRuntimeAgent::Client implementation.
    void resumeStartup() override;
    bool isRunRequired() override;

    OwnPtr<InspectorStateClient> m_stateClient;
    OwnPtrWillBeMember<InspectorCompositeState> m_state;
    OwnPtrWillBeMember<InjectedScriptManager> m_injectedScriptManager;
    OwnPtrWillBeMember<WorkerThreadDebugger> m_workerThreadDebugger;
    InspectorAgentRegistry m_agents;
    InspectorFrontendChannel* m_frontendChannel;
    OwnPtr<InspectorFrontend> m_frontend;
    RefPtrWillBeMember<InspectorBackendDispatcher> m_backendDispatcher;
    RawPtrWillBeMember<WorkerDebuggerAgent> m_workerDebuggerAgent;
    RawPtrWillBeMember<WorkerRuntimeAgent> m_workerRuntimeAgent;
    bool m_paused;
};

}

#endif // V8Inspector_h
