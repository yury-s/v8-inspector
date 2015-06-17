// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef REMOTE_DEBUGGING_SERVER_H_
#define REMOTE_DEBUGGING_SERVER_H_

#include "net/server/http_server.h"

namespace net {

//class ServerWrapper;

class RemoteDebuggingServer {
public:
    static void* createServer();
};

}  // namespace net

#endif // REMOTE_DEBUGGING_SERVER_H_
