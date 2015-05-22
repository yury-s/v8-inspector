// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8Inspector_h
#define V8Inspector_h

#include "core/inspector/InspectorFrontendChannel.h"
#include "core/inspector/InspectorRuntimeAgent.h"
#include "core/inspector/InspectorStateClient.h"
#include "wtf/Forward.h"
#include "wtf/OwnPtr.h"

namespace blink {

class V8InspectorClient;

class V8Inspector final
    : public InspectorStateClient
    , public InspectorRuntimeAgent::Client
    , public InspectorFrontendChannel {
public:
    explicit V8Inspector(V8InspectorClient*);
    ~V8Inspector();

    void attach(const String& hostId);
    void reattach(const String& hostId, const String& savedState);
    void detach();
    void dispatchOnInspectorBackend(const String& message);

private:
    // InspectorStateClient implementation.
    void updateInspectorStateCookie(const String&) override;

    // InspectorRuntimeAgent::Client implementation.
    void resumeStartup() override { }

    // InspectorFrontendChannel implementation.
    void sendProtocolResponse(int callId, PassRefPtr<JSONObject> message) override;
    void sendProtocolNotification(PassRefPtr<JSONObject> message) override;
    void flush() override { }

    V8InspectorClient* m_client;
    bool m_attached;

    RefPtr<InstrumentingAgents> m_instrumentingAgents;
    OwnPtr<InjectedScriptManager> m_injectedScriptManager;
    OwnPtr<InspectorCompositeState> m_state;
    OwnPtr<AsyncCallTracker> m_asyncCallTracker;

    RefPtr<InspectorBackendDispatcher> m_inspectorBackendDispatcher;
    OwnPtr<InspectorFrontend> m_inspectorFrontend;
    InspectorAgentRegistry m_agents;
    String m_stateCookie;
    friend class DebuggerTask;
};

} // namespace blink

#endif
