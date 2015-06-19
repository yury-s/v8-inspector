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

namespace v8inspector {

 
namespace {

// Maximum write buffer size of devtools http/websocket connections.
// TODO(rmcilroy/pfieldman): Reduce this back to 100Mb when we have
// added back pressure on the TraceComplete message protocol - crbug.com/456845.
const int32 kSendBufferSizeForDevTools = 256 * 1024 * 1024;  // 256Mb

// HttpServerDelegate -------------------------------------------------------------
// All methods in this class are only called on handler thread.
class ServerWrapper : net::HttpServer::Delegate {
 public:
  explicit ServerWrapper(scoped_ptr<net::ServerSocket> socket);

  int GetLocalAddress(net::IPEndPoint* address);

  void AcceptWebSocket(int connection_id,
                       const net::HttpServerRequestInfo& request);
  void SendOverWebSocket(int connection_id, const std::string& message);
  void SendResponse(int connection_id,
                    const net::HttpServerResponseInfo& response);
  void Send200(int connection_id,
               const std::string& data,
               const std::string& mime_type);
  void Send404(int connection_id);
  void Send500(int connection_id, const std::string& message);
  void Close(int connection_id);

  virtual ~ServerWrapper() {}

 private:
  // net::HttpServer::Delegate implementation.
  void OnConnect(int connection_id) override {}
  void OnHttpRequest(int connection_id,
                     const net::HttpServerRequestInfo& info) override;
  void OnWebSocketRequest(int connection_id,
                          const net::HttpServerRequestInfo& info) override;
  void OnWebSocketMessage(int connection_id,
                          const std::string& data) override;
  void OnClose(int connection_id) override;

  scoped_ptr<net::HttpServer> server_;
};

ServerWrapper::ServerWrapper(scoped_ptr<net::ServerSocket> socket)
    : server_(new net::HttpServer(socket.Pass(), this)) {
}

int ServerWrapper::GetLocalAddress(net::IPEndPoint* address) {
  return server_->GetLocalAddress(address);
}

void ServerWrapper::AcceptWebSocket(int connection_id,
                                    const net::HttpServerRequestInfo& request) {
  server_->SetSendBufferSize(connection_id, kSendBufferSizeForDevTools);
  server_->AcceptWebSocket(connection_id, request);
}

void ServerWrapper::SendOverWebSocket(int connection_id,
                                      const std::string& message) {
  server_->SendOverWebSocket(connection_id, message);
}

void ServerWrapper::SendResponse(int connection_id,
                                 const net::HttpServerResponseInfo& response) {
  server_->SendResponse(connection_id, response);
}

void ServerWrapper::Send200(int connection_id,
                            const std::string& data,
                            const std::string& mime_type) {
  server_->Send200(connection_id, data, mime_type);
}

void ServerWrapper::Send404(int connection_id) {
  server_->Send404(connection_id);
}

void ServerWrapper::Send500(int connection_id,
                            const std::string& message) {
  server_->Send500(connection_id, message);
}

void ServerWrapper::Close(int connection_id) {
  server_->Close(connection_id);
}



void ServerWrapper::OnHttpRequest(int connection_id, const net::HttpServerRequestInfo& info) {
    fprintf(stderr, "ServerWrapper::OnHttpRequest 1\n");
}

void ServerWrapper::OnWebSocketRequest(int connection_id, const net::HttpServerRequestInfo& info) {
    fprintf(stderr, "ServerWrapper::OnWebSocketRequest 1\n");
    AcceptWebSocket(connection_id, info);
    fprintf(stderr, "ServerWrapper::OnWebSocketRequest 2\n");
}

void ServerWrapper::OnWebSocketMessage(int connection_id, const std::string& data) {
    fprintf(stderr, "ServerWrapper::OnWebSocketMessage 1 m = %s\n", data.data());
}

void ServerWrapper::OnClose(int connection_id) {
    fprintf(stderr, "ServerWrapper::OnClose\n");
}

}

static void ioTask(int n)
{
  fprintf(stderr, "**** ioTask n = %d\n", n);
}

RemoteDebuggingServer::RemoteDebuggingServer()
    : thread_(nullptr)
    , main_thread_loop_(base::MessageLoop::current())
{
    thread_.reset(new base::Thread("IO/Handler Thread"));
    base::Thread::Options options;
    options.message_loop_type = base::MessageLoop::TYPE_IO;
    if (thread_->StartWithOptions(options)) {
        base::MessageLoop* message_loop = thread_->message_loop();
        message_loop->task_runner()->PostTask(FROM_HERE, base::Bind(ioTask, 1));
        message_loop->task_runner()->PostTask(
            FROM_HERE,
            base::Bind(&RemoteDebuggingServer::StartServerOnHandlerThread,
                       base::Unretained(this)));
        message_loop->task_runner()->PostTask(FROM_HERE, base::Bind(ioTask, 2));
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
    ServerWrapper* server_wrapper = new ServerWrapper(server_socket.Pass());
    fprintf(stderr, "RemoteDebuggingServer::StartServerOnHandlerThread 3\n");
    if (server_wrapper->GetLocalAddress(ip_address.get()) != net::OK) {
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



void* RemoteDebuggingServer::createServer()
{
    scoped_ptr<net::ServerSocket> server_socket(
        new net::TCPServerSocket(nullptr, net::NetLog::Source()));
    fprintf(stderr, "createServer 1\n");
    if (server_socket->ListenWithAddressAndPort("127.0.0.1", 2015, 10) != net::OK) {
    fprintf(stderr, "createServer 2\n");
      return nullptr;
    }
    scoped_ptr<net::IPEndPoint> ip_address(new net::IPEndPoint);
    fprintf(stderr, "createServer 3\n");
    ServerWrapper* server_wrapper = new ServerWrapper(server_socket.Pass());
    fprintf(stderr, "createServer 4\n");
    if (server_wrapper->GetLocalAddress(ip_address.get()) != net::OK) {
      fprintf(stderr, "createServer 5\n");
      ip_address.reset();
    }
    fprintf(stderr, "createServer 6\n");
    return nullptr;
}

}  // namespace net
