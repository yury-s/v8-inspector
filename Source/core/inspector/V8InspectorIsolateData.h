// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8InspectorIsolateData_h
#define V8InspectorIsolateData_h

#include "wtf/Noncopyable.h"
#include "wtf/RefPtr.h"
#include <v8.h>

namespace blink {

class ScriptState;

class V8InspectorIsolateData final {
    WTF_MAKE_NONCOPYABLE(V8InspectorIsolateData);
public:
    static V8InspectorIsolateData* from(v8::Isolate*);

    v8::Local<v8::Context> ensureScriptRegexpContext();

private:
    explicit V8InspectorIsolateData(v8::Isolate*);
    ~V8InspectorIsolateData();

    v8::Isolate* m_isolate;
    RefPtr<ScriptState> m_scriptRegexpScriptState;
};

} // namespace blink


#endif // !defined(V8InspectorIsolateData_h)
