// Copyright (c) 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef InspectorMessageLoop_h
#define InspectorMessageLoop_h

#include "base/callback.h"
#include <list>

class InspectorMessageLoop {
 public:
  InspectorMessageLoop();
  ~InspectorMessageLoop();

  void run();
  void postTask(const base::Closure&);
  static InspectorMessageLoop* current();

 private:
  std::list<base::Closure> queue_;
  static InspectorMessageLoop* message_loop_;

  DISALLOW_COPY_AND_ASSIGN(InspectorMessageLoop);
};

#endif  // InspectorMessageLoop_h
