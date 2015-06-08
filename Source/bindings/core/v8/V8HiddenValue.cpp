// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"
#include "bindings/core/v8/V8HiddenValue.h"

namespace blink {

#define V8_DEFINE_METHOD(name) \
v8::Local<v8::String> V8HiddenValue::name(v8::Isolate* isolate)    \
{ \
    return v8::String::NewFromUtf8(isolate, #name, v8::NewStringType::kInternalized).ToLocalChecked();\
}

V8_HIDDEN_VALUES(V8_DEFINE_METHOD);

v8::Local<v8::Value> V8HiddenValue::getHiddenValue(v8::Isolate* isolate, v8::Local<v8::Object> object, v8::Local<v8::String> key)
{
    return object->GetHiddenValue(key);
}

bool V8HiddenValue::setHiddenValue(v8::Isolate* isolate, v8::Local<v8::Object> object, v8::Local<v8::String> key, v8::Local<v8::Value> value)
{
    return object->SetHiddenValue(key, value);
}

bool V8HiddenValue::deleteHiddenValue(v8::Isolate* isolate, v8::Local<v8::Object> object, v8::Local<v8::String> key)
{
    return object->DeleteHiddenValue(key);
}

} // namespace blink
