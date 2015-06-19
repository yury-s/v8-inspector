// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTE_DEBUGGING_SERVER_H_
#define REMOTE_DEBUGGING_SERVER_H_

#include "base/memory/scoped_ptr.h"
#include "net/server/http_server.h"

namespace base {
class Thread;
class MessageLoop;
}

namespace v8inspector {

class RemoteDebuggingServer : public net::HttpServer::Delegate {
public:
    RemoteDebuggingServer();
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

    scoped_ptr<base::Thread> io_thread_;
    base::MessageLoop* main_thread_loop_;
    scoped_ptr<net::HttpServer> http_server_;
};

}  // namespace net

#endif // REMOTE_DEBUGGING_SERVER_H_
