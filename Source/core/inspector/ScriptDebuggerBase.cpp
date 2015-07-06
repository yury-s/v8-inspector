// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"
#include "core/inspector/ScriptDebuggerBase.h"

#include "bindings/core/v8/V8Binding.h"
#include "bindings/core/v8/V8ScriptRunner.h"

namespace blink {

String ScriptDebuggerBase::loadFromFile(const char* name)
{
  FILE* file = fopen(name, "rb");
  if (file == NULL) {
      fprintf(stderr, "File not found (%s).", name);
      return String();
  }

  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  rewind(file);

  OwnPtr<char[]> arrayHolder = adoptArrayPtr(new char[size + 1]);
  char* chars = arrayHolder.get();
  chars[size] = '\0';
  for (size_t i = 0; i < size;) {
    i += fread(&chars[i], 1, size - i, file);
    if (ferror(file)) {
      fclose(file);
      fprintf(stderr, "Error reading from file (%s).", name);
      return String();
    }
  }
  fclose(file);
  return String(chars, size);
}

ScriptDebuggerBase::ScriptDebuggerBase(v8::Isolate* isolate, PassOwnPtrWillBeRawPtr<V8Debugger> debugger)
    : m_isolate(isolate)
    , m_debugger(debugger)
{
}

ScriptDebuggerBase::~ScriptDebuggerBase()
{
}

v8::Local<v8::Object> ScriptDebuggerBase::compileDebuggerScript()
{
    const char* relativePath = "./deps/WebKit/Source/bindings/core/v8/DebuggerScript.js";
    String debuggerScript = ScriptDebuggerBase::loadFromFile(relativePath);
    fprintf(stderr, "Loaded debugger script from %s\n", relativePath);
    v8::Local<v8::String> source = v8String(m_isolate, debuggerScript);
    v8::Local<v8::Value> value;
    if (!V8ScriptRunner::compileAndRunInternalScript(source, m_isolate).ToLocal(&value))
        return v8::Local<v8::Object>();
    ASSERT(value->IsObject());
    return value.As<v8::Object>();
}

}
