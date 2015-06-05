/*
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "bindings/core/v8/V8ScriptRunner.h"

#include "bindings/core/v8/V8Binding.h"

namespace blink {

v8::MaybeLocal<v8::Script> V8ScriptRunner::compileScript(v8::Local<v8::String> code, const String& fileName, const String& sourceMapUrl, const TextPosition& scriptStartPosition, v8::Isolate* isolate, bool isInternalScript)
{
    // NOTE: For compatibility with WebCore, ScriptSourceCode's line starts at
    // 1, whereas v8 starts at 0.
    v8::ScriptOrigin origin(
        v8String(isolate, fileName),
        v8::Integer::New(isolate, scriptStartPosition.m_line.zeroBasedInt()),
        v8::Integer::New(isolate, scriptStartPosition.m_column.zeroBasedInt()),
        v8Boolean(false, isolate),
        v8::Local<v8::Integer>(),
        v8Boolean(isInternalScript, isolate));


    v8::ScriptCompiler::Source source(code, origin);
    v8::MaybeLocal<v8::Script> script = v8::ScriptCompiler::Compile(isolate->GetCurrentContext(), &source, v8::ScriptCompiler::kNoCompileOptions);
    return script;
}

v8::MaybeLocal<v8::Value> V8ScriptRunner::runCompiledScript(v8::Isolate* isolate, v8::Local<v8::Script> script)
{
    ASSERT(!script.IsEmpty());

    // Run the script and keep track of the current recursion depth.
    v8::MaybeLocal<v8::Value> result = script->Run(isolate->GetCurrentContext());

    crashIfV8IsDead();
    return result;
}

v8::MaybeLocal<v8::Value> V8ScriptRunner::compileAndRunInternalScript(v8::Local<v8::String> source, v8::Isolate* isolate, const String& fileName, const TextPosition& scriptStartPosition)
{
    v8::Local<v8::Script> script;
    if (!V8ScriptRunner::compileScript(source, fileName, String(), scriptStartPosition, isolate, true).ToLocal(&script))
        return v8::MaybeLocal<v8::Value>();

    v8::MaybeLocal<v8::Value> result = script->Run(isolate->GetCurrentContext());
    crashIfV8IsDead();
    return result;
}

v8::MaybeLocal<v8::Value> V8ScriptRunner::callFunction(v8::Local<v8::Function> function, v8::Local<v8::Value> receiver, int argc, v8::Local<v8::Value> args[], v8::Isolate* isolate)
{
    v8::MaybeLocal<v8::Value> result = function->Call(isolate->GetCurrentContext(), receiver, argc, args);
    crashIfV8IsDead();
    return result;
}

v8::MaybeLocal<v8::Value> V8ScriptRunner::callInternalFunction(v8::Local<v8::Function> function, v8::Local<v8::Value> receiver, int argc, v8::Local<v8::Value> args[], v8::Isolate* isolate)
{
    v8::MaybeLocal<v8::Value> result = function->Call(isolate->GetCurrentContext(), receiver, argc, args);
    crashIfV8IsDead();
    return result;
}

v8::MaybeLocal<v8::Object> V8ScriptRunner::instantiateObject(v8::Isolate* isolate, v8::Local<v8::Function> function, int argc, v8::Local<v8::Value> argv[])
{
    v8::MaybeLocal<v8::Object> result = function->NewInstance(isolate->GetCurrentContext(), argc, argv);
    crashIfV8IsDead();
    return result;
}

} // namespace blink
