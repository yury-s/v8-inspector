// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8InspectorThreads_h
#define V8InspectorThreads_h

namespace base {
class MessageLoop;
}

namespace v8inspector {

class V8InspectorThreads {
public:

    static base::MessageLoop* mainThreadLoop() { return m_mainThreadLoop; }
    static base::MessageLoop* ioThreadLoop() { return m_ioThreadLoop; }
    static void setMainThreadLoop(base::MessageLoop* loop) { m_mainThreadLoop = loop; }
    static void setIOThreadLoop(base::MessageLoop* loop) { m_mainThreadLoop = loop; }

private:
    static base::MessageLoop* m_mainThreadLoop;
    static base::MessageLoop* m_ioThreadLoop;
};

}

#endif // V8InspectorThreads_h
