/*
 * Copyright (c) 2011 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WorkerThreadDebugger_h
#define WorkerThreadDebugger_h

#include "core/inspector/ScriptDebuggerBase.h"
#include "platform/heap/Handle.h"
#include "wtf/Forward.h"

#include <v8.h>

namespace blink {

class WorkerThreadDebugger final : public NoBaseWillBeGarbageCollectedFinalized<WorkerThreadDebugger>, public ScriptDebuggerBase {
    WTF_MAKE_NONCOPYABLE(WorkerThreadDebugger);
    WILL_BE_USING_GARBAGE_COLLECTED_MIXIN(WorkerThreadDebugger);
public:
    class ClientMessageLoop {
    public:
        virtual ~ClientMessageLoop() { }
        virtual void run() = 0;
        virtual void quitNow() = 0;
    };

    static PassOwnPtrWillBeRawPtr<WorkerThreadDebugger> create(v8::Isolate* isolate, PassOwnPtr<ClientMessageLoop> clientMessageLoop)

    {
        return adoptPtrWillBeNoop(new WorkerThreadDebugger(isolate, clientMessageLoop));
    }

    ~WorkerThreadDebugger() override;

    static void setContextDebugData(v8::Local<v8::Context>);
    void addListener(ScriptDebugListener*);
    void removeListener(ScriptDebugListener*);

private:
    explicit WorkerThreadDebugger(v8::Isolate*, PassOwnPtr<ClientMessageLoop>);

    ScriptDebugListener* getDebugListenerForContext(v8::Local<v8::Context>) override;
    void runMessageLoopOnPause(v8::Local<v8::Context>) override;
    void quitMessageLoopOnPause() override;

    OwnPtr<ClientMessageLoop> m_clientMessageLoop;
    ScriptDebugListener* m_listener;
};

} // namespace blink

#endif // WorkerThreadDebugger_h
