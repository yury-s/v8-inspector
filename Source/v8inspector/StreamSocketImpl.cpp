// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "v8inspector/StreamSocketImpl.h"

#include "v8inspector/InspectorMessageLoop.h"
#include "net/base/io_buffer.h"
#include "net/base/net_errors.h"

#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>

namespace net {

StreamSocketImpl::StreamSocketImpl(int client_socket) : client_socket_(client_socket) {

//     {
//       struct linger so_linger;
//       so_linger.l_onoff = 0;
//       so_linger.l_linger = 0;
//       int z = setsockopt(client_socket, SOL_SOCKET, SO_LINGER, &so_linger, sizeof so_linger);
//       if ( z )
//           perror("setsockopt(2)");
//     }

}

StreamSocketImpl::~StreamSocketImpl() {
}

int StreamSocketImpl::Read(IOBuffer* buf, int buf_len, const CompletionCallback& callback) {
//    fprintf(stderr, "StreamSocketImpl Read buf_len = %d\n", buf_len);
    //Receive a message from client
    int read_size = recv(client_socket_, buf->data() , buf_len , 0);
//    printf("recv message read_size = %d message = %s\n", read_size,  buf->data());
    InspectorMessageLoop::current()->postTask(base::Bind(callback, read_size));
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
    return read_size;
}

int StreamSocketImpl::Write(IOBuffer* buf, int buf_len, const CompletionCallback& callback) {
    fprintf(stderr, "StreamSocketImpl Write %d %s\n", buf_len, buf->data());
    //Send the message back to client
    ssize_t len = write(client_socket_ , buf->data(), buf_len);
    return len;
}


}  // namespace net
