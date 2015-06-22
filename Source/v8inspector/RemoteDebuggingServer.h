// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTE_DEBUGGING_SERVER_H_
#define REMOTE_DEBUGGING_SERVER_H_

#include "base/memory/scoped_ptr.h"
#include "core/inspector/InspectorFrontendChannel.h"
#include "net/server/http_server.h"

namespace base {
class Thread;
class MessageLoop;
}

namespace blink {
class V8Inspector;
}

namespace v8inspector {

class RemoteDebuggingServer : public net::HttpServer::Delegate, public blink::InspectorFrontendChannel {
public:
    explicit RemoteDebuggingServer(blink::V8Inspector*);
    virtual ~RemoteDebuggingServer();

private:
    void StartServerOnHandlerThread();

    // net::HttpServer::Delegate implementation.
    void OnConnect(int connection_id) override {}
    void OnHttpRequest(int connection_id,
                       const net::HttpServerRequestInfo& info) override;
    void OnWebSocketRequest(int connection_id,
                            const net::HttpServerRequestInfo& info) override;
    void OnWebSocketMessage(int connection_id,
                            const std::string& data) override;
    void OnClose(int connection_id) override;

    // Protocol implementation.
    void HandleConnect(int connection_id);
    void HandleMessageFromClient(int connection_id, const std::string& data);
    void HandleDisconnect(int connection_id);

    // InspectorFrontendChannel implementation.
    void sendProtocolResponse(int callId, PassRefPtr<blink::JSONObject> message) override;
    void sendProtocolNotification(PassRefPtr<blink::JSONObject> message) override;
    void flush() override {}

    void serializeAndSend(PassRefPtr<blink::JSONObject> message);

    // Send* methods. Called on the IO thread.
    void sendMessageToClient(const std::string& message);

    blink::V8Inspector* inspector_;
    scoped_ptr<base::Thread> io_thread_;
    base::MessageLoop* main_thread_loop_;
    scoped_ptr<net::HttpServer> http_server_;
    int connection_id_;
};

}  // namespace net

#endif // REMOTE_DEBUGGING_SERVER_H_
