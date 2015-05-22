// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8InspectorClient_h
#define V8InspectorClient_h

#include "wtf/Forward.h"

namespace blink {

class V8InspectorClient {
public:
    // Sends response message over the protocol, update agent state on the browser side for
    // potential re-attach. |callId| for notifications is 0, |state| for notifications is empty.
    virtual void sendProtocolMessage(int callId, const String& response, const String& state) { }

    // Returns process id.
    virtual long processId() { return -1; }

    // Returns unique identifier of the entity within process.
    virtual int debuggerId() { return -1; }

    class ClientMessageLoop {
    public:
        virtual ~ClientMessageLoop() { }
        virtual void run() = 0;
        virtual void quitNow() = 0;
    };
    virtual ClientMessageLoop* createClientMessageLoop() { return 0; }

protected:
    ~V8InspectorClient() { }
};

} // namespace blink

#endif
