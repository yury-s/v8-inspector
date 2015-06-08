// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef ScriptState_h
#define ScriptState_h

#include "bindings/core/v8/ScopedPersistent.h"
#include "core/CoreExport.h"
#include "platform/heap/Handle.h"
#include "wtf/RefCounted.h"
#include "wtf/Vector.h"
#include <v8-debug.h>
#include <v8.h>

namespace blink {

class ScriptValue;

// ScriptState is created when v8::Context is created.
// ScriptState is destroyed when v8::Context is garbage-collected and
// all V8 proxy objects that have references to the ScriptState are destructed.
class CORE_EXPORT ScriptState : public RefCounted<ScriptState> {
    WTF_MAKE_NONCOPYABLE(ScriptState);
public:
    class Scope {
    public:
        // You need to make sure that scriptState->context() is not empty before creating a Scope.
        explicit Scope(ScriptState* scriptState)
            : m_handleScope(scriptState->isolate())
            , m_context(scriptState->context())
        {
            ASSERT(scriptState->contextIsValid());
            m_context->Enter();
        }

        ~Scope()
        {
            m_context->Exit();
        }

    private:
        v8::HandleScope m_handleScope;
        v8::Local<v8::Context> m_context;
    };

    static PassRefPtr<ScriptState> create(v8::Local<v8::Context>);
    virtual ~ScriptState();

    static ScriptState* current(v8::Isolate* isolate)
    {
        return from(isolate->GetCurrentContext());
    }

    // Debugger context doesn't have associated ScriptState and when current
    // context is debugger it should be treated as if context stack was empty.
    static bool hasCurrentScriptState(v8::Isolate* isolate)
    {
        v8::HandleScope scope(isolate);
        v8::Local<v8::Context> context = isolate->GetCurrentContext();
        if (context.IsEmpty())
            return false;
        return context != v8::Debug::GetDebugContext();
    }

    static ScriptState* from(v8::Local<v8::Context> context)
    {
        ASSERT(!context.IsEmpty());
        ScriptState* scriptState = static_cast<ScriptState*>(context->GetAlignedPointerFromEmbedderData(v8ContextPerContextDataIndex));
        // ScriptState::from() must not be called for a context that does not have
        // valid embedder data in the embedder field.
        RELEASE_ASSERT_WITH_SECURITY_IMPLICATION(scriptState);
        RELEASE_ASSERT_WITH_SECURITY_IMPLICATION(scriptState->context() == context);
        return scriptState;
    }

    v8::Isolate* isolate() const { return m_isolate; }

    // This can return an empty handle if the v8::Context is gone.
    v8::Local<v8::Context> context() const { return m_context.newLocal(m_isolate); }
    bool contextIsValid() const { return !m_context.isEmpty(); }
    void detachGlobalObject();
    void clearContext() { return m_context.clear(); }
#if ENABLE(ASSERT)
    bool isGlobalObjectDetached() const { return m_globalObjectDetached; }
#endif

    void disposePerContextData();

    class Observer {
    public:
        virtual ~Observer() { }
        virtual void willDisposeScriptState(ScriptState*) = 0;
    };
    void addObserver(Observer*);
    void removeObserver(Observer*);

    bool evalEnabled() const;
    void setEvalEnabled(bool);

protected:
    explicit ScriptState(v8::Local<v8::Context>);

private:
    static int v8ContextPerContextDataIndex;
    v8::Isolate* m_isolate;
    // This persistent handle is weak.
    ScopedPersistent<v8::Context> m_context;

#if ENABLE(ASSERT)
    bool m_globalObjectDetached;
#endif
    Vector<Observer*> m_observers;
};

// ScriptStateProtectingContext keeps the context associated with the ScriptState alive.
// You need to call clear() once you no longer need the context. Otherwise, the context will leak.
class ScriptStateProtectingContext {
    WTF_MAKE_NONCOPYABLE(ScriptStateProtectingContext);
public:
    ScriptStateProtectingContext(ScriptState* scriptState)
        : m_scriptState(scriptState)
    {
        if (m_scriptState)
            m_context.set(m_scriptState->isolate(), m_scriptState->context());
    }

    ScriptState* operator->() const { return m_scriptState.get(); }
    ScriptState* get() const { return m_scriptState.get(); }
    void clear()
    {
        m_scriptState = nullptr;
        m_context.clear();
    }

private:
    RefPtr<ScriptState> m_scriptState;
    ScopedPersistent<v8::Context> m_context;
};

}

#endif // ScriptState_h
