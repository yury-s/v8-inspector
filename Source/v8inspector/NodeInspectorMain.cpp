// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"

#include "bindings/core/v8/ScriptState.h"
#include "bindings/core/v8/WorkerThreadDebugger.h"
#include "v8inspector/V8Inspector.h"
#include "wtf/OwnPtr.h"

#include <include/v8.h>
#include <include/libplatform/libplatform.h>

#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace blink;

namespace {

// class DebuggerMessageLoopImpl : public WorkerThreadDebugger::ClientMessageLoop {
//  public:
//   DebuggerMessageLoopImpl() : message_loop_(base::MessageLoop::current()) {}
//   virtual ~DebuggerMessageLoopImpl() { message_loop_ = NULL; }
//   void run() override {
//     base::MessageLoop::ScopedNestableTaskAllower allow(message_loop_);
//     message_loop_->Run();
//   }
//   void quitNow() override {
//     message_loop_->QuitNow();
//   }
//  private:
//   base::MessageLoop* message_loop_;
// };

}

int main(int argc, char* argv[]) {
//   v8::Platform* platform = v8::platform::CreateDefaultPlatform();
//   v8::V8::InitializePlatform(platform);
  fprintf(stderr, "main 10\n");
    // Set context deubg data to see its scripts in the debugger.
//    WorkerThreadDebugger::setContextDebugData(context);
//    OwnPtr<V8Inspector> inspector = adoptPtr(new V8Inspector(isolate, adoptPtr(new DebuggerMessageLoopImpl())));
    fprintf(stderr, "V8 inspector is running\n");
//  delete platform;
  return 0;
}
