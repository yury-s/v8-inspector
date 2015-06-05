// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"
#include "core/inspector/V8InspectorIsolateData.h"

#include "bindings/core/v8/DOMWrapperWorld.h"
#include "bindings/core/v8/ScriptState.h"

namespace blink {


V8InspectorIsolateData* V8InspectorIsolateData::from(v8::Isolate* isolate)
{
    // FIXME
    static V8InspectorIsolateData* data = new V8InspectorIsolateData(isolate);
    ASSERT(isolate == data->m_isolate);
    return data;
}

V8InspectorIsolateData::V8InspectorIsolateData(v8::Isolate* isolate) : m_isolate(isolate)
{
}

V8InspectorIsolateData::~V8InspectorIsolateData()
{
}

v8::Local<v8::Context> V8InspectorIsolateData::ensureScriptRegexpContext()
{
    if (!m_scriptRegexpScriptState) {
        v8::Local<v8::Context> context(v8::Context::New(m_isolate));
        m_scriptRegexpScriptState = ScriptState::create(context, nullptr);
    }
    return m_scriptRegexpScriptState->context();
}

} // namespace blink
