// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "v8inspector/InspectorMessageLoop.h"

using namespace base;

InspectorMessageLoop* InspectorMessageLoop::message_loop_ = nullptr;

InspectorMessageLoop::InspectorMessageLoop() {
  message_loop_ = this;
}

InspectorMessageLoop::~InspectorMessageLoop() {
  message_loop_ = nullptr;
}

void InspectorMessageLoop::postTask(const Closure& task)
{
  queue_.push_back(task);
}

void InspectorMessageLoop::run()
{
    fprintf(stderr, "InspectorMessageLoop::run>>\n");
  while (!queue_.empty()) {
    
    base::Closure task = queue_.front();
    fprintf(stderr, "InspectorMessageLoop::run run next task>>\n");
    task.Run();
    queue_.pop_front();
  }
    fprintf(stderr, "InspectorMessageLoop::run<<\n");
}


InspectorMessageLoop* InspectorMessageLoop::current()
{
  return message_loop_;
}
