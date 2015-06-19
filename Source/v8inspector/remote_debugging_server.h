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

class RemoteDebuggingServer {
public:
    RemoteDebuggingServer();
    ~RemoteDebuggingServer();


    static void* createServer();

private:
    void StartServerOnHandlerThread();

    scoped_ptr<base::Thread> thread_;
    base::MessageLoop* main_thread_loop_;
};

}  // namespace net

#endif // REMOTE_DEBUGGING_SERVER_H_
