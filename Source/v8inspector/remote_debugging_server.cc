// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "v8inspector/remote_debugging_server.h"

#include "base/run_loop.h"
#include "net/base/net_errors.h"
#include "net/http/http_request_info.h"
#include "net/server/http_server_request_info.h"
#include "net/server/http_server.h"
#include "net/socket/tcp_server_socket.h"
#include "v8inspector/ServerSocketImpl.h"

namespace net {

 
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
  fprintf(stderr, "ServerWrapper server_ = %p\n", server_.get());
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
    fprintf(stderr, "ServerWrapper::OnHttpRequest %s %s\n", info.method.data(), info.path.data());
    Send200(connection_id, "Hello!", "text/plain");
}

void ServerWrapper::OnWebSocketRequest(int connection_id, const net::HttpServerRequestInfo& info) {
    fprintf(stderr, "ServerWrapper::OnWebSocketRequest\n");
}

void ServerWrapper::OnWebSocketMessage(int connection_id, const std::string& data) {
    fprintf(stderr, "ServerWrapper::OnWebSocketMessage\n");
}

void ServerWrapper::OnClose(int connection_id) {
    fprintf(stderr, "ServerWrapper::OnClose\n");
}

}

void* RemoteDebuggingServer::createServer()
{
    scoped_ptr<net::ServerSocket> server_socket(new ServerSocketImpl());
    if (server_socket->ListenWithAddressAndPort("127.0.0.1", 9223, 10) != net::OK) {
      return nullptr;
    }
    ServerWrapper* server_wrapper = new ServerWrapper(server_socket.Pass());
    fprintf(stderr, "RemoteDebuggingServer::createServer\n");
    return server_wrapper;
}

}  // namespace net
