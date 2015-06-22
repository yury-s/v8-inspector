// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"

#include "v8inspector/RemoteDebuggingServer.h"

#include "base/message_loop/message_loop.h"
#include "base/run_loop.h"
#include "base/threading/thread.h"
#include "base/bind.h"
#include "net/base/net_errors.h"
#include "net/server/http_server.h"
#include "net/socket/tcp_server_socket.h"
#include "v8inspector/V8Inspector.h"
#include "wtf/text/StringUTF8Adaptor.h"
#include <string>

using namespace blink;

namespace v8inspector {


namespace {

std::string wtfToStdString(const String& string)
{
    StringUTF8Adaptor utf8(string);
    return std::string(utf8.data(), utf8.length());
}

}
 
// Maximum write buffer size of devtools http/websocket connections.
// TODO(rmcilroy/pfieldman): Reduce this back to 100Mb when we have
// added back pressure on the TraceComplete message protocol - crbug.com/456845.
static const int32 kSendBufferSizeForDevTools = 256 * 1024 * 1024;  // 256Mb

// net::HttpServer::Delegate implementation -------------------------------------------------------------
// All methods in the delegate are only called on handler thread.
void RemoteDebuggingServer::OnHttpRequest(int connection_id, const net::HttpServerRequestInfo& request) {
    fprintf(stderr, "RemoteDebuggingServer::OnHttpRequest 1\n");
}

void RemoteDebuggingServer::OnWebSocketRequest(int connection_id, const net::HttpServerRequestInfo& request) {
    http_server_->SetSendBufferSize(connection_id, kSendBufferSizeForDevTools);
    http_server_->AcceptWebSocket(connection_id, request);
    ASSERT(connection_id_ == -1);
    connection_id_ = connection_id;
    fprintf(stderr, "RemoteDebuggingServer::OnWebSocketRequest accepted.\n");
    main_thread_loop_->task_runner()->PostTask(
        FROM_HERE,
        base::Bind(&RemoteDebuggingServer::HandleConnect,
                   base::Unretained(this), connection_id));
}

void RemoteDebuggingServer::OnWebSocketMessage(int connection_id, const std::string& data) {
    main_thread_loop_->task_runner()->PostTask(
        FROM_HERE,
        base::Bind(&RemoteDebuggingServer::HandleMessageFromClient,
                   base::Unretained(this), connection_id, data));
}

void RemoteDebuggingServer::OnClose(int connection_id) {
    connection_id_ = -1;
    fprintf(stderr, "RemoteDebuggingServer::OnClose\n");
    main_thread_loop_->task_runner()->PostTask(
        FROM_HERE,
        base::Bind(&RemoteDebuggingServer::HandleDisconnect,
                   base::Unretained(this), connection_id));
}

// Actual implementation. These methods are called on the main (JavaScript) thread.
void RemoteDebuggingServer::HandleConnect(int connection_id)
{
    fprintf(stderr, "RemoteDebuggingServer::HandleConnect\n");
    inspector_->connectFrontend(this);
}

void RemoteDebuggingServer::HandleMessageFromClient(int connection_id, const std::string& data)
{
    fprintf(stderr, "RemoteDebuggingServer::HandleMessageFromClient %s\n", data.substr(0, 100).data());
    String message = String::fromUTF8(data.data(), data.length());
    inspector_->dispatchMessageFromFrontend(message);
}

void RemoteDebuggingServer::HandleDisconnect(int connection_id)
{
    fprintf(stderr, "RemoteDebuggingServer::HandleDisconnect\n");
    inspector_->disconnectFrontend();
}

// InspectorFrontendChannel implementation.
void RemoteDebuggingServer::sendProtocolResponse(int callId, PassRefPtr<JSONObject> message)
{
//    printf("ChannelImpl::sendProtocolResponse \n");
    printf("ChannelImpl::sendProtocolResponse callId = %d message = %s\n", callId, message->toPrettyJSONString().utf8().data());
    serializeAndSend(message);
}

void RemoteDebuggingServer::sendProtocolNotification(PassRefPtr<JSONObject> message)
{
    printf("ChannelImpl::sendProtocolNotification \n");
//         printf("ChannelImpl::sendProtocolNotification message = %s\n", message->toPrettyJSONString().utf8().data());
    serializeAndSend(message);
}

void RemoteDebuggingServer::serializeAndSend(PassRefPtr<blink::JSONObject> message)
{
    String responseString = message->toJSONString();
    std::string response = wtfToStdString(responseString);

    io_thread_->message_loop()->task_runner()->PostTask(
        FROM_HERE,
        base::Bind(&RemoteDebuggingServer::sendMessageToClient,
                   base::Unretained(this), response));
}


// Send methods. Called on the IO thread.
void RemoteDebuggingServer::sendMessageToClient(const std::string& message)
{
    if (connection_id_ == -1) {
        printf("RemoteDebuggingServer::sendMessageToClient failed, connection closed \n");
        return;
    }
    http_server_->SendOverWebSocket(connection_id_, message);
}

RemoteDebuggingServer::RemoteDebuggingServer(V8Inspector* inspector)
    : inspector_(inspector)
    , io_thread_(nullptr)
    , main_thread_loop_(base::MessageLoop::current())
    , connection_id_(-1)
{
    io_thread_.reset(new base::Thread("IO/Handler Thread"));
    base::Thread::Options options;
    options.message_loop_type = base::MessageLoop::TYPE_IO;
    if (io_thread_->StartWithOptions(options)) {
        base::MessageLoop* message_loop = io_thread_->message_loop();
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
    scoped_ptr<net::IPEndPoint> ip_address(new net::IPEndPoint);
    http_server_.reset(new net::HttpServer(server_socket.Pass(), this));
    if (http_server_->GetLocalAddress(ip_address.get()) != net::OK) {
        fprintf(stderr, "RemoteDebuggingServer::StartServerOnHandlerThread 4 failed to GetLocalAddress\n");
        ip_address.reset();
    }
    fprintf(stderr, "RemoteDebuggingServer::StartServerOnHandlerThread Done.\n");
}

}  // namespace net
