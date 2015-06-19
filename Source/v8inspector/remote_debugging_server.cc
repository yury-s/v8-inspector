// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "v8inspector/remote_debugging_server.h"

#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "base/threading/thread.h"
#include "base/bind.h"
#include "net/base/net_errors.h"
#include "net/server/http_server.h"
#include "net/socket/tcp_server_socket.h"
#include "v8inspector/V8InspectorThreads.h"

namespace v8inspector {
 
namespace {

// Maximum write buffer size of devtools http/websocket connections.
// TODO(rmcilroy/pfieldman): Reduce this back to 100Mb when we have
// added back pressure on the TraceComplete message protocol - crbug.com/456845.
const int32 kSendBufferSizeForDevTools = 256 * 1024 * 1024;  // 256Mb

}

// net::HttpServer::Delegate implementation -------------------------------------------------------------
// All methods in the delegate are only called on handler thread.
void RemoteDebuggingServer::OnHttpRequest(int connection_id, const net::HttpServerRequestInfo& request) {
    fprintf(stderr, "HttpServerDelegateImpl::OnHttpRequest 1\n");
}

void RemoteDebuggingServer::OnWebSocketRequest(int connection_id, const net::HttpServerRequestInfo& request) {
    http_server_->SetSendBufferSize(connection_id, kSendBufferSizeForDevTools);
    http_server_->AcceptWebSocket(connection_id, request);
    fprintf(stderr, "HttpServerDelegateImpl::OnWebSocketRequest 2\n");
}

void RemoteDebuggingServer::OnWebSocketMessage(int connection_id, const std::string& data) {
    fprintf(stderr, "HttpServerDelegateImpl::OnWebSocketMessage 1 m = %s\n", data.data());
}

void RemoteDebuggingServer::OnClose(int connection_id) {
    fprintf(stderr, "HttpServerDelegateImpl::OnClose\n");
}

RemoteDebuggingServer::RemoteDebuggingServer()
    : io_thread_(nullptr)
    , main_thread_loop_(base::MessageLoop::current())
{
    io_thread_.reset(new base::Thread("IO/Handler Thread"));
    base::Thread::Options options;
    options.message_loop_type = base::MessageLoop::TYPE_IO;
    if (io_thread_->StartWithOptions(options)) {
        base::MessageLoop* message_loop = io_thread_->message_loop();
        V8InspectorThreads::setIOThreadLoop(message_loop);
        message_loop->task_runner()->PostTask(
            FROM_HERE,
            base::Bind(&RemoteDebuggingServer::StartServerOnHandlerThread,
                       base::Unretained(this)));
    }
}

RemoteDebuggingServer::~RemoteDebuggingServer()
{
}

void RemoteDebuggingServer::StartServerOnHandlerThread()
{
    scoped_ptr<net::ServerSocket> server_socket(
        new net::TCPServerSocket(nullptr, net::NetLog::Source()));
    if (server_socket->ListenWithAddressAndPort("127.0.0.1", 2015, 10) != net::OK) {
        fprintf(stderr, "RemoteDebuggingServer::StartServerOnHandlerThread FAILED to start listen socket\n");
        return;
    }
    fprintf(stderr, "RemoteDebuggingServer::StartServerOnHandlerThread 2\n");
    scoped_ptr<net::IPEndPoint> ip_address(new net::IPEndPoint);
    http_server_.reset(new net::HttpServer(server_socket.Pass(), this));
    fprintf(stderr, "RemoteDebuggingServer::StartServerOnHandlerThread 3\n");
    if (http_server_->GetLocalAddress(ip_address.get()) != net::OK) {
        fprintf(stderr, "RemoteDebuggingServer::StartServerOnHandlerThread 4 failed to GetLocalAddress\n");
        ip_address.reset();
    }
    fprintf(stderr, "RemoteDebuggingServer::StartServerOnHandlerThread 5\n");
//     BrowserThread::PostTask(BrowserThread::UI, FROM_HERE,
//         base::Bind(&ServerStartedOnUI,
//                    handler,
//                    thread,
//                    server_wrapper,
//                    server_socket_factory,
//                    base::Passed(&ip_address)));
}

}  // namespace net
