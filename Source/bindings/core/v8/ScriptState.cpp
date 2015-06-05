// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"
#include "bindings/core/v8/ScriptState.h"

#include "bindings/core/v8/V8Binding.h"

namespace blink {

PassRefPtr<ScriptState> ScriptState::create(v8::Local<v8::Context> context)
{
    RefPtr<ScriptState> scriptState = adoptRef(new ScriptState(context));
    // This ref() is for keeping this ScriptState alive as long as the v8::Context is alive.
    // This is deref()ed in the weak callback of the v8::Context.
    scriptState->ref();
    return scriptState;
}

static void derefCallback(const v8::WeakCallbackInfo<ScriptState>& data)
{
    data.GetParameter()->deref();
}

static void weakCallback(const v8::WeakCallbackInfo<ScriptState>& data)
{
    data.GetParameter()->clearContext();
    data.SetSecondPassCallback(derefCallback);
}


int ScriptState::v8ContextPerContextDataIndex = 2;

ScriptState::ScriptState(v8::Local<v8::Context> context)
    : m_isolate(context->GetIsolate())
    , m_context(m_isolate, context)
#if ENABLE(ASSERT)
    , m_globalObjectDetached(false)
#endif
{
    m_context.setWeak(this, &weakCallback);

    context->SetAlignedPointerInEmbedderData(v8ContextPerContextDataIndex, this);
}

ScriptState::~ScriptState()
{
    ASSERT(m_context.isEmpty());
}

void ScriptState::detachGlobalObject()
{
    ASSERT(!m_context.isEmpty());
    context()->DetachGlobal();
#if ENABLE(ASSERT)
    m_globalObjectDetached = true;
#endif
}

void ScriptState::disposePerContextData()
{
    Vector<Observer*> observers(m_observers);
    for (auto& observer : observers)
        observer->willDisposeScriptState(this);
}

void ScriptState::addObserver(Observer* observer)
{
    m_observers.append(observer);
}

void ScriptState::removeObserver(Observer* observer)
{
    size_t index = m_observers.find(observer);
    if (index != kNotFound)
        m_observers.remove(index);
}

bool ScriptState::evalEnabled() const
{
    v8::HandleScope handleScope(m_isolate);
    return context()->IsCodeGenerationFromStringsAllowed();
}

void ScriptState::setEvalEnabled(bool enabled)
{
    v8::HandleScope handleScope(m_isolate);
    return context()->AllowCodeGenerationFromStrings(enabled);
}

ScriptValue ScriptState::getFromGlobalObject(const char* name)
{
    v8::HandleScope handleScope(m_isolate);
    v8::Local<v8::Value> v8Value;
    if (!context()->Global()->Get(context(), v8AtomicString(isolate(), name)).ToLocal(&v8Value))
        return ScriptValue();
    return ScriptValue(this, v8Value);
}

}
