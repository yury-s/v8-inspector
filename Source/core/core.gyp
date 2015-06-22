#
# Copyright (C) 2009 Google Inc. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#     * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#     * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#

{
  'includes': [
    '../build/win/precompile.gypi',
    '../build/features.gypi',
    '../build/scripts/scripts.gypi',
  ],

  'variables': {
    'enable_wexit_time_destructors': 1,

    'webcore_include_dirs': [
      '..',  # WebKit/Source
    ],
  },  # variables

  'target_defaults': {
    'variables': {
      'optimize': 'max',
    },
  },

  'targets': [
    {
      # GN version: //third_party/WebKit/Source/core:core
      'target_name': 'webcore_v8inspector',
      'type': 'static_library',
      'dependencies': [
        # instead of 'webcore_prerequisites':
        '../chrome/url/url.gyp:url_lib', # this one is required due to ScriptStreamer.h
        '../chrome/base/base.gyp:base',

        # Exported.
        '../wtf/wtf.gyp:wtf',
        '../chrome/v8/tools/gyp/v8.gyp:v8',
      ],
      'include_dirs': [
        '<@(webcore_include_dirs)',
        '../..', # blink root for includes like 'public/platform/WebThread.h'
      ],
      'sources': [
        'Init.cpp',
        'Init.h',
        'inspector/AsyncCallChain.cpp',
        'inspector/AsyncCallChain.h',
        'inspector/AsyncOperationMap.h',
        'inspector/ContentSearchUtils.cpp',
        'inspector/ContentSearchUtils.h',
        'inspector/InjectedScript.cpp',
        'inspector/InjectedScript.h',
        'inspector/InjectedScriptNative.cpp',
        'inspector/InjectedScriptNative.h',
        'inspector/InjectedScriptBase.cpp',
        'inspector/InjectedScriptBase.h',
        'inspector/InjectedScriptHost.cpp',
        'inspector/InjectedScriptHost.h',
        'inspector/InjectedScriptManager.cpp',
        'inspector/InjectedScriptManager.h',
        'inspector/InspectorBaseAgent.cpp',
        'inspector/InspectorDebuggerAgent.cpp',
        'inspector/InspectorDebuggerAgent.h',
        'inspector/InspectorFrontendChannel.h',
        'inspector/InspectorRuntimeAgent.cpp',
        'inspector/InspectorRuntimeAgent.h',
        'inspector/InspectorState.cpp',
        'inspector/InspectorState.h',
        'inspector/InspectorStateClient.h',
        'inspector/JSONParser.cpp',
        'inspector/JSONParser.h',
        'inspector/JavaScriptCallFrame.cpp',
        'inspector/JavaScriptCallFrame.h',
        'inspector/PromiseTracker.cpp',
        'inspector/PromiseTracker.h',
        'inspector/ScriptArguments.cpp',
        'inspector/ScriptArguments.h',
        'inspector/ScriptAsyncCallStack.cpp',
        'inspector/ScriptAsyncCallStack.h',
        'inspector/ScriptCallFrame.cpp',
        'inspector/ScriptCallFrame.h',
        'inspector/ScriptCallStack.cpp',
        'inspector/ScriptCallStack.h',
        'inspector/ScriptDebuggerBase.cpp',
        'inspector/ScriptDebuggerBase.h',
        'inspector/ScriptDebugListener.cpp',
        'inspector/ScriptDebugListener.h',
        'inspector/V8AsyncCallTracker.cpp',
        'inspector/V8AsyncCallTracker.h',
        'inspector/WorkerDebuggerAgent.cpp',
        'inspector/WorkerDebuggerAgent.h',
        'inspector/WorkerRuntimeAgent.cpp',
        'inspector/WorkerRuntimeAgent.h',

        '../bindings/core/v8/ScopedPersistent.h',
        '../bindings/core/v8/ScriptCallStackFactory.cpp',
        '../bindings/core/v8/ScriptCallStackFactory.h',
        '../bindings/core/v8/ScriptFunctionCall.cpp',
        '../bindings/core/v8/ScriptFunctionCall.h',
        '../bindings/core/v8/ScriptRegexp.cpp',
        '../bindings/core/v8/ScriptRegexp.h',
        '../bindings/core/v8/ScriptSourceCode.cpp',
        '../bindings/core/v8/ScriptSourceCode.h',
        '../bindings/core/v8/ScriptState.cpp',
        '../bindings/core/v8/ScriptState.h',
        '../bindings/core/v8/ScriptValue.cpp',
        '../bindings/core/v8/ScriptValue.h',
        '../bindings/core/v8/SharedPersistent.h',

        '../bindings/core/v8/V8Debugger.cpp',
        '../bindings/core/v8/V8Debugger.h',
        '../bindings/core/v8/WorkerThreadDebugger.cpp',
        '../bindings/core/v8/WorkerThreadDebugger.h',

        '../bindings/core/v8/custom/V8InjectedScriptManager.cpp',
        '../bindings/core/v8/inspector/InspectorWrapper.cpp',
        '../bindings/core/v8/inspector/InspectorWrapper.h',
        '../bindings/core/v8/inspector/V8InjectedScriptHost.cpp',
        '../bindings/core/v8/inspector/V8InjectedScriptHost.h',
        '../bindings/core/v8/inspector/V8JavaScriptCallFrame.cpp',
        '../bindings/core/v8/inspector/V8JavaScriptCallFrame.h',

        # generated files
        '../core/InspectorBackendDispatcher.cpp',
        '../core/InspectorBackendDispatcher.h',
        '../core/InspectorFrontend.cpp',
        '../core/InspectorFrontend.h',
        '../core/InspectorTypeBuilder.cpp',
        '../core/InspectorTypeBuilder.h',

        # platform
        '../platform/JSONValues.cpp',
        '../platform/JSONValues.h',
        '../platform/Decimal.cpp',
        '../platform/Decimal.h',

        # newly added in the experiment
        '../core/inspector/V8InspectorIsolateData.cpp',
        '../core/inspector/V8InspectorIsolateData.h',

        # to be removed
        '../bindings/core/v8/V8Binding.cpp',
        '../bindings/core/v8/V8Binding.h',
        '../bindings/core/v8/V8ScriptRunner.cpp',
        '../bindings/core/v8/V8ScriptRunner.h',
      ],
    },
  ],  # targets
}
