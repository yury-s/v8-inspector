// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "v8inspector/ServerSocketImpl.h"

#include "net/base/net_errors.h"
#include "v8inspector/InspectorMessageLoop.h"
#include "v8inspector/StreamSocketImpl.h"

#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>    //write

namespace net {

ServerSocketImpl::ServerSocketImpl() {
  init();
}

ServerSocketImpl::~ServerSocketImpl() {
}

void ServerSocketImpl::init() {
    sockaddr_in server;
    //Create socket
    socket_desc_ = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc_ == -1)
    {
        printf("Could not create socket");
        return;
    }
    puts("Socket created");
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
    //Bind
    if( bind(socket_desc_,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return;
    }
    puts("bind done");
    //Listen
    listen(socket_desc_ , 3);

    //Accept and incoming connection
    puts("Waiting for incoming connections...");
}


  // Binds the socket and starts listening. Destroys the socket to stop
  // listening.
int ServerSocketImpl::Listen(const IPEndPoint& address, int backlog) {
    fprintf(stderr, "ServerSocketImpl::Listen\n");
    return net::OK;
}

  // Gets current address the socket is bound to.
int ServerSocketImpl::GetLocalAddress(IPEndPoint* address) const {
    fprintf(stderr, "ServerSocketImpl::GetLocalAddress\n");
    return -1;
}

int ServerSocketImpl::Accept(scoped_ptr<StreamSocket>* socket, const CompletionCallback& callback) {
    sockaddr_in client;
    int c = sizeof(struct sockaddr_in);
    fprintf(stderr, "ServerSocketImpl::Accept waiting\n");
    //accept connection from an incoming client
    int client_sock = accept(socket_desc_, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return net::ERR_FAILED;
    }
    fprintf(stderr, "ServerSocketImpl::Accept created socket %d\n", client_sock);
    socket->reset(new StreamSocketImpl(client_sock));
    InspectorMessageLoop::current()->postTask(base::Bind(callback, net::OK));
//    return net::ERR_IO_PENDING;
    return net::OK;
}

}  // namespace net
