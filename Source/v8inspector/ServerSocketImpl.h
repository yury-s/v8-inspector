// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ServerSocketImpl_h
#define ServerSocketImpl_h

#include "net/socket/server_socket.h"

namespace net {

class ServerSocketImpl : public ServerSocket {
 public:
  ServerSocketImpl();
  virtual ~ServerSocketImpl();

  // Binds the socket and starts listening. Destroys the socket to stop
  // listening.
  int Listen(const IPEndPoint& address, int backlog) override;

  // Gets current address the socket is bound to.
  int GetLocalAddress(IPEndPoint* address) const override;

  // Accepts connection. Callback is called when new connection is
  // accepted.
  int Accept(scoped_ptr<StreamSocket>* socket, const CompletionCallback& callback) override;

 private:
  DISALLOW_COPY_AND_ASSIGN(ServerSocketImpl);
};

}  // namespace net

#endif  // ServerSocketImpl_h
