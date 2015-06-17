// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "v8inspector/ServerSocketImpl.h"

#include "net/base/net_errors.h"
#include "v8inspector/InspectorMessageLoop.h"
#include "v8inspector/StreamSocketImpl.h"

namespace net {

ServerSocketImpl::ServerSocketImpl() {
}

ServerSocketImpl::~ServerSocketImpl() {
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

  // Accepts connection. Callback is called when new connection is
  // accepted.
int ServerSocketImpl::Accept(scoped_ptr<StreamSocket>* socket,
                     const CompletionCallback& callback) {
    socket->reset(new StreamSocketImpl());
    fprintf(stderr, "ServerSocketImpl::Accept\n");
    InspectorMessageLoop::current()->postTask(base::Bind(callback, 0));
    return net::ERR_IO_PENDING;
}

}  // namespace net
