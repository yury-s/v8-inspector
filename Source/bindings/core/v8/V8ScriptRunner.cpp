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
#include "bindings/core/v8/V8RecursionScope.h"
#include "bindings/core/v8/V8ThrowException.h"
#include "platform/ScriptForbiddenScope.h"
#include "platform/TraceEvent.h"
#include "public/platform/Platform.h"
#include "wtf/CurrentTime.h"

#if defined(WTF_OS_WIN)
#include <malloc.h>
#else
#include <alloca.h>
#endif

namespace blink {

namespace {

// Used to throw an exception before we exceed the C++ stack and crash.
// This limit was arrived at arbitrarily. crbug.com/449744
const int kMaxRecursionDepth = 44;

// In order to make sure all pending messages to be processed in
// v8::Function::Call, we don't call throwStackOverflowException
// directly. Instead, we create a v8::Function of
// throwStackOverflowException and call it.
void throwStackOverflowException(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8ThrowException::throwRangeError(info.GetIsolate(), "Maximum call stack size exceeded.");
}

void throwScriptForbiddenException(v8::Isolate* isolate)
{
    V8ThrowException::throwGeneralError(isolate, "Script execution is forbidden.");
}

v8::Local<v8::Value> throwStackOverflowExceptionIfNeeded(v8::Isolate* isolate)
{
    if (V8PerIsolateData::from(isolate)->isHandlingRecursionLevelError()) {
        // If we are already handling a recursion level error, we should
        // not invoke v8::Function::Call.
        return v8::Undefined(isolate);
    }
    V8PerIsolateData::from(isolate)->setIsHandlingRecursionLevelError(true);
    v8::Local<v8::Value> result = v8::Function::New(isolate, throwStackOverflowException)->Call(v8::Undefined(isolate), 0, 0);
    V8PerIsolateData::from(isolate)->setIsHandlingRecursionLevelError(false);
    return result;
}

} // namespace

v8::MaybeLocal<v8::Script> V8ScriptRunner::compileScript(v8::Local<v8::String> code, const String& fileName, const String& sourceMapUrl, const TextPosition& scriptStartPosition, v8::Isolate* isolate, bool isInternalScript)
{
    AccessControlStatus accessControlStatus;

    // NOTE: For compatibility with WebCore, ScriptSourceCode's line starts at
    // 1, whereas v8 starts at 0.
    v8::ScriptOrigin origin(
        v8String(isolate, fileName),
        v8::Integer::New(isolate, scriptStartPosition.m_line.zeroBasedInt()),
        v8::Integer::New(isolate, scriptStartPosition.m_column.zeroBasedInt()),
        v8Boolean(accessControlStatus == SharableCrossOrigin, isolate),
        v8::Local<v8::Integer>(),
        v8Boolean(isInternalScript, isolate),
        v8String(isolate, sourceMapUrl),
        v8Boolean(accessControlStatus == OpaqueResource, isolate));


    v8::ScriptCompiler::Source source(code, origin);
    v8::MaybeLocal<v8::Script> script = v8::ScriptCompiler::Compile(isolate->GetCurrentContext(), &source, v8::ScriptCompiler::kNoCompileOptions);
    return script;
}

v8::MaybeLocal<v8::Value> V8ScriptRunner::runCompiledScript(v8::Isolate* isolate, v8::Local<v8::Script> script, ExecutionContext* context)
{
    ASSERT(!script.IsEmpty());

    if (V8RecursionScope::recursionLevel(isolate) >= kMaxRecursionDepth)
        return throwStackOverflowExceptionIfNeeded(isolate);

    RELEASE_ASSERT(!context->isIteratingOverObservers());

    // Run the script and keep track of the current recursion depth.
    v8::MaybeLocal<v8::Value> result;
    {
        if (ScriptForbiddenScope::isScriptForbidden()) {
            throwScriptForbiddenException(isolate);
            return v8::MaybeLocal<v8::Value>();
        }
        V8RecursionScope recursionScope(isolate);
        result = script->Run(isolate->GetCurrentContext());
    }

    crashIfV8IsDead();
    return result;
}

v8::MaybeLocal<v8::Value> V8ScriptRunner::compileAndRunInternalScript(v8::Local<v8::String> source, v8::Isolate* isolate, const String& fileName, const TextPosition& scriptStartPosition)
{
    v8::Local<v8::Script> script;
    if (!V8ScriptRunner::compileScript(source, fileName, String(), scriptStartPosition, isolate, true).ToLocal(&script))
        return v8::MaybeLocal<v8::Value>();

    V8RecursionScope::MicrotaskSuppression recursionScope(isolate);
    v8::MaybeLocal<v8::Value> result = script->Run(isolate->GetCurrentContext());
    crashIfV8IsDead();
    return result;
}

v8::MaybeLocal<v8::Value> V8ScriptRunner::callFunction(v8::Local<v8::Function> function, ExecutionContext* context, v8::Local<v8::Value> receiver, int argc, v8::Local<v8::Value> args[], v8::Isolate* isolate)
{
    if (V8RecursionScope::recursionLevel(isolate) >= kMaxRecursionDepth)
        return v8::MaybeLocal<v8::Value>(throwStackOverflowExceptionIfNeeded(isolate));

    RELEASE_ASSERT(!context->isIteratingOverObservers());

    if (ScriptForbiddenScope::isScriptForbidden()) {
        throwScriptForbiddenException(isolate);
        return v8::MaybeLocal<v8::Value>();
    }
    V8RecursionScope recursionScope(isolate);
    v8::MaybeLocal<v8::Value> result = function->Call(isolate->GetCurrentContext(), receiver, argc, args);
    crashIfV8IsDead();
    return result;
}

v8::MaybeLocal<v8::Value> V8ScriptRunner::callInternalFunction(v8::Local<v8::Function> function, v8::Local<v8::Value> receiver, int argc, v8::Local<v8::Value> args[], v8::Isolate* isolate)
{
    V8RecursionScope::MicrotaskSuppression recursionScope(isolate);
    v8::MaybeLocal<v8::Value> result = function->Call(isolate->GetCurrentContext(), receiver, argc, args);
    crashIfV8IsDead();
    return result;
}

v8::MaybeLocal<v8::Object> V8ScriptRunner::instantiateObject(v8::Isolate* isolate, v8::Local<v8::Function> function, int argc, v8::Local<v8::Value> argv[])
{
    V8RecursionScope::MicrotaskSuppression scope(isolate);
    v8::MaybeLocal<v8::Object> result = function->NewInstance(isolate->GetCurrentContext(), argc, argv);
    crashIfV8IsDead();
    return result;
}

} // namespace blink
