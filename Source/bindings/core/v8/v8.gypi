# Copyright 2014 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
    'includes': [
        'custom/custom.gypi',
        '../../modules/v8/generated.gypi',
    ],
    'variables': {
        'bindings_core_v8_dir': '.',
        'bindings_core_v8_files': [
            '<@(bindings_core_v8_custom_files)',
            'ActiveDOMCallback.cpp',
            'ActiveDOMCallback.h',
            'ArrayValue.cpp',
            'ArrayValue.h',
            'BindingSecurity.cpp',
            'BindingSecurity.h',
            'CallbackPromiseAdapter.h',
            'CustomElementBinding.cpp',
            'CustomElementBinding.h',
            'CustomElementConstructorBuilder.cpp',
            'CustomElementConstructorBuilder.h',
            'DOMDataStore.h',
            'DOMWrapperMap.h',
            'DOMWrapperWorld.cpp',
            'DOMWrapperWorld.h',
            'Dictionary.cpp',
            'Dictionary.h',
            'DictionaryHelperForBindings.h',
            'DictionaryHelperForCore.cpp',
            'ExceptionMessages.cpp',
            'ExceptionMessages.h',
            'ExceptionState.cpp',
            'ExceptionState.h',
            'ExceptionStatePlaceholder.cpp',
            'ExceptionStatePlaceholder.h',
            'Iterable.h',
            'MainThreadDebugger.cpp',
            'MainThreadDebugger.h',
            'Maplike.h',
            'ModuleProxy.cpp',
            'ModuleProxy.h',
            'NativeValueTraits.h',
            'NPV8Object.cpp',
            'NPV8Object.h',
            'Nullable.h',
            'OnStackObjectChecker.cpp',
            'OnStackObjectChecker.h',
            'PostMessage.h',
            'PrivateScriptRunner.cpp',
            'PrivateScriptRunner.h',
            'RejectedPromises.cpp',
            'RejectedPromises.h',
            'RetainedDOMInfo.cpp',
            'RetainedDOMInfo.h',
            'RetainedObjectInfo.h',
            'ScheduledAction.cpp',
            'ScheduledAction.h',
            'ScopedPersistent.h',
            'ScriptCallStackFactory.cpp',
            'ScriptCallStackFactory.h',
            'ScriptController.cpp',
            'ScriptController.h',
            'ScriptEventListener.cpp',
            'ScriptEventListener.h',
            'ScriptFunction.cpp',
            'ScriptFunction.h',
            'ScriptFunctionCall.cpp',
            'ScriptFunctionCall.h',
            'ScriptHeapSnapshot.cpp',
            'ScriptHeapSnapshot.h',
            'ScriptProfiler.cpp',
            'ScriptProfiler.h',
            'ScriptPromise.cpp',
            'ScriptPromise.h',
            'ScriptPromiseProperties.h',
            'ScriptPromiseProperty.h',
            'ScriptPromisePropertyBase.cpp',
            'ScriptPromisePropertyBase.h',
            'ScriptPromiseResolver.cpp',
            'ScriptPromiseResolver.h',
            'ScriptRegexp.cpp',
            'ScriptRegexp.h',
            'ScriptSourceCode.cpp',
            'ScriptSourceCode.h',
            'ScriptState.cpp',
            'ScriptState.h',
            'ScriptStreamer.cpp',
            'ScriptStreamer.h',
            'ScriptStreamerThread.cpp',
            'ScriptStreamerThread.h',
            'ScriptString.cpp',
            'ScriptString.h',
            'ScriptValue.cpp',
            'ScriptValue.h',
            'ScriptValueSerializer.cpp',
            'ScriptValueSerializer.h',
            'ScriptWrappable.cpp',
            'ScriptWrappable.h',
            'SerializationTag.h',
            'SerializedScriptValue.cpp',
            'SerializedScriptValue.h',
            'SerializedScriptValueFactory.cpp',
            'SerializedScriptValueFactory.h',
            'SharedPersistent.h',
            'ToV8.cpp',
            'ToV8.h',
            'V8AbstractEventListener.cpp',
            'V8AbstractEventListener.h',
            'V8Binding.cpp',
            'V8Binding.h',
            'V8BindingMacros.h',
            'V8CacheOptions.h',
            'V8CustomElementLifecycleCallbacks.cpp',
            'V8CustomElementLifecycleCallbacks.h',
            'V8Debugger.cpp',
            'V8Debugger.h',
            'V8DOMActivityLogger.cpp',
            'V8DOMActivityLogger.h',
            'V8DOMConfiguration.cpp',
            'V8DOMConfiguration.h',
            'V8DOMWrapper.cpp',
            'V8DOMWrapper.h',
            'V8ErrorHandler.cpp',
            'V8ErrorHandler.h',
            'V8EventListener.cpp',
            'V8EventListener.h',
            'V8EventListenerList.cpp',
            'V8EventListenerList.h',
            'V8GCController.cpp',
            'V8GCController.h',
            'V8GCForContextDispose.cpp',
            'V8GCForContextDispose.h',
            'V8GlobalValueMap.h',
            'V8HiddenValue.cpp',
            'V8HiddenValue.h',
            'V8Initializer.cpp',
            'V8Initializer.h',
            'V8IteratorResultValue.cpp',
            'V8IteratorResultValue.h',
            'V8LazyEventListener.cpp',
            'V8LazyEventListener.h',
            'V8MutationCallback.cpp',
            'V8MutationCallback.h',
            'V8NPObject.cpp',
            'V8NPObject.h',
            'V8NPUtils.cpp',
            'V8NPUtils.h',
            'V8NodeFilterCondition.cpp',
            'V8NodeFilterCondition.h',
            'V8ObjectBuilder.cpp',
            'V8ObjectBuilder.h',
            'V8ObjectConstructor.cpp',
            'V8ObjectConstructor.h',
            'V8PagePopupControllerBinding.cpp',
            'V8PagePopupControllerBinding.h',
            'V8PerContextData.cpp',
            'V8PerContextData.h',
            'V8PerIsolateData.cpp',
            'V8PerIsolateData.h',
            'V8PersistentValueVector.h',
            'V8RecursionScope.cpp',
            'V8RecursionScope.h',
            'V8ScriptRunner.cpp',
            'V8ScriptRunner.h',
            'V8StringResource.cpp',
            'V8StringResource.h',
            'V8ThrowException.cpp',
            'V8ThrowException.h',
            'V8ValueCache.cpp',
            'V8ValueCache.h',
            'V8WorkerGlobalScopeEventListener.cpp',
            'V8WorkerGlobalScopeEventListener.h',
            'WebGLAny.cpp',
            'WebGLAny.h',
            'WindowProxy.cpp',
            'WindowProxy.h',
            'WindowProxyManager.cpp',
            'WindowProxyManager.h',
            'WorkerScriptController.cpp',
            'WorkerScriptController.h',
            'WorkerThreadDebugger.cpp',
            'WorkerThreadDebugger.h',
            'WrapCanvasContext.cpp',
            'WrapCanvasContext.h',
            'WrapperTypeInfo.cpp',
            'WrapperTypeInfo.h',
            'npruntime.cpp',
            'npruntime_impl.h',
            'npruntime_priv.h',
        ],
        'bindings_core_v8_unittest_files': [
            'ScriptPromisePropertyTest.cpp',
            'ScriptPromiseResolverTest.cpp',
            'ScriptPromiseTest.cpp',
            'ScriptStreamerTest.cpp',
            'SerializedScriptValueTest.cpp',
            'ToV8Test.cpp',
            'V8BindingForTesting.cpp',
            'V8BindingForTesting.h',
            'V8BindingTest.cpp',
            'V8ScriptRunnerTest.cpp',
        ],
    },
}
