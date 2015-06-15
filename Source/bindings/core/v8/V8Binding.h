/*
* Copyright (C) 2009 Google Inc. All rights reserved.
* Copyright (C) 2012 Ericsson AB. All rights reserved.
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

#ifndef V8Binding_h
#define V8Binding_h

#include "bindings/core/v8/V8BindingMacros.h"
#include "platform/heap/Handle.h"
#include "wtf/text/AtomicString.h"
#include <v8.h>

namespace blink {

template<typename CallbackInfo, typename S>
inline void v8SetReturnValue(const CallbackInfo& info, const v8::Persistent<S>& handle)
{
    info.GetReturnValue().Set(handle);
}

template<typename CallbackInfo, typename S>
inline void v8SetReturnValue(const CallbackInfo& info, const v8::Local<S> handle)
{
    info.GetReturnValue().Set(handle);
}

template<typename CallbackInfo, typename S>
inline void v8SetReturnValue(const CallbackInfo& info, v8::MaybeLocal<S> maybe)
{
    if (LIKELY(!maybe.IsEmpty()))
        info.GetReturnValue().Set(maybe.ToLocalChecked());
}

template<typename CallbackInfo>
inline void v8SetReturnValue(const CallbackInfo& info, bool value)
{
    info.GetReturnValue().Set(value);
}

template<typename CallbackInfo>
inline void v8SetReturnValue(const CallbackInfo& info, double value)
{
    info.GetReturnValue().Set(value);
}

template<typename CallbackInfo>
inline void v8SetReturnValue(const CallbackInfo& info, int32_t value)
{
    info.GetReturnValue().Set(value);
}

template<typename CallbackInfo>
inline void v8SetReturnValue(const CallbackInfo& info, uint32_t value)
{
    info.GetReturnValue().Set(value);
}

template<typename CallbackInfo>
inline void v8SetReturnValueBool(const CallbackInfo& info, bool v)
{
    info.GetReturnValue().Set(v);
}

template<typename CallbackInfo>
inline void v8SetReturnValueInt(const CallbackInfo& info, int v)
{
    info.GetReturnValue().Set(v);
}

template<typename CallbackInfo>
inline void v8SetReturnValueUnsigned(const CallbackInfo& info, unsigned v)
{
    info.GetReturnValue().Set(v);
}

template<typename CallbackInfo>
inline void v8SetReturnValueNull(const CallbackInfo& info)
{
    info.GetReturnValue().SetNull();
}

template<typename CallbackInfo>
inline void v8SetReturnValueUndefined(const CallbackInfo& info)
{
    info.GetReturnValue().SetUndefined();
}

template<typename CallbackInfo>
inline void v8SetReturnValueEmptyString(const CallbackInfo& info)
{
    info.GetReturnValue().SetEmptyString();
}

// Convert v8::String to a WTF::String. If the V8 string is not already
// an external string then it is transformed into an external string at this
// point to avoid repeated conversions.
inline String toCoreString(v8::Local<v8::String> value)
{
    v8::String::Utf8Value utf8(value);
    return String(*utf8, utf8.length());
}

inline String toCoreStringWithNullCheck(v8::Local<v8::String> value)
{
    if (value.IsEmpty() || value->IsNull())
        return String();
    return toCoreString(value);
}

// This method will return a null String if the v8::Value does not contain a v8::String.
// It will not call ToString() on the v8::Value. If you want ToString() to be called,
// please use the TONATIVE_FOR_V8STRINGRESOURCE_*() macros instead.
inline String toCoreStringWithUndefinedOrNullCheck(v8::Local<v8::Value> value)
{
    if (value.IsEmpty() || !value->IsString())
        return String();
    return toCoreString(value.As<v8::String>());
}

// Convert a string to a V8 string.
// Return a V8 external string that shares the underlying buffer with the given
// WebCore string. The reference counting mechanism is used to keep the
// underlying buffer alive while the string is still live in the V8 engine.
inline v8::Local<v8::String> v8String(v8::Isolate* isolate, const String& string)
{
    if (string.isNull())
        return v8::String::Empty(isolate);
    // FIXME
    return v8::String::NewFromUtf8(isolate, string.utf8().data(), v8::String::kNormalString, string.utf8().length());
}

inline v8::Local<v8::String> v8AtomicString(v8::Isolate* isolate, const char* str, int length = -1)
{
    ASSERT(isolate);
    v8::Local<v8::String> value;
    if (LIKELY(v8::String::NewFromUtf8(isolate, str, v8::NewStringType::kInternalized, length).ToLocal(&value)))
        return value;
    // Immediately crashes when NewFromUtf8() fails because it only fails the
    // given str is too long.
    RELEASE_ASSERT_NOT_REACHED();
    return v8::String::Empty(isolate);
}

inline v8::Local<v8::Value> v8Undefined()
{
    return v8::Local<v8::Value>();
}

inline v8::Local<v8::Boolean> v8Boolean(bool value, v8::Isolate* isolate)
{
    return value ? v8::True(isolate) : v8::False(isolate);
}

// If the current context causes out of memory, JavaScript setting
// is disabled and it returns true.
void crashIfV8IsDead();

v8::Local<v8::Function> getBoundFunction(v8::Local<v8::Function>);

class DevToolsFunctionInfo final {
public:
    explicit DevToolsFunctionInfo(v8::Local<v8::Function>& function)
        : m_scriptId(0)
        , m_lineNumber(1)
        , m_function(function)
    {
        ASSERT(!m_function.IsEmpty());
    }

    DevToolsFunctionInfo(int scriptId, const String& resourceName, int lineNumber)
        : m_scriptId(scriptId)
        , m_lineNumber(lineNumber)
        , m_resourceName(resourceName)
    {
    }

    int scriptId() const;
    int lineNumber() const;
    String resourceName() const;

private:
    void ensureInitialized() const;

    mutable int m_scriptId;
    mutable int m_lineNumber;
    mutable String m_resourceName;
    mutable v8::Local<v8::Function> m_function;
};



} // namespace blink

#endif // V8Binding_h
