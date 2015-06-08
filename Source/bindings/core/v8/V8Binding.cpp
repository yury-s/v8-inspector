/*
 * Copyright (C) 2006, 2007, 2008, 2009 Google Inc. All rights reserved.
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

#include "config.h"
#include "bindings/core/v8/V8Binding.h"

#include "wtf/text/AtomicString.h"
#include "wtf/text/CString.h"
#include "wtf/text/StringBuffer.h"
#include "wtf/text/StringHash.h"
#include "wtf/text/WTFString.h"
#include "wtf/unicode/CharacterNames.h"
#include "wtf/unicode/Unicode.h"

namespace blink {

void crashIfV8IsDead()
{
    if (v8::V8::IsDead()) {
        // FIXME: We temporarily deal with V8 internal error situations
        // such as out-of-memory by crashing the renderer.
        CRASH();
    }
}

v8::Local<v8::Function> getBoundFunction(v8::Local<v8::Function> function)
{
    v8::Local<v8::Value> boundFunction = function->GetBoundFunction();
    return boundFunction->IsFunction() ? v8::Local<v8::Function>::Cast(boundFunction) : function;
}

void DevToolsFunctionInfo::ensureInitialized() const
{
    if (m_function.IsEmpty())
        return;

    v8::HandleScope scope(m_function->GetIsolate());
    v8::Local<v8::Function> originalFunction = getBoundFunction(m_function);
    m_scriptId = originalFunction->ScriptId();
//     v8::ScriptOrigin origin = originalFunction->GetScriptOrigin();
//     if (!origin.ResourceName().IsEmpty()) {
//         V8StringResource<> stringResource(origin.ResourceName());
//         stringResource.prepare();
//         m_resourceName = stringResource;
//         m_lineNumber = originalFunction->GetScriptLineNumber() + 1;
//     }
//     if (m_resourceName.isEmpty()) {
//         m_resourceName = "";
//         m_lineNumber = 1;
//     }
        m_resourceName = "FIXME";
        m_lineNumber = 1;

    m_function.Clear();
}

int DevToolsFunctionInfo::scriptId() const
{
    ensureInitialized();
    return m_scriptId;
}

int DevToolsFunctionInfo::lineNumber() const
{
    ensureInitialized();
    return m_lineNumber;
}

String DevToolsFunctionInfo::resourceName() const
{
    // FIXME
    return m_resourceName;
}

} // namespace blink
