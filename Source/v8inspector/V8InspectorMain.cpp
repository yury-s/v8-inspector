// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"

#include "v8inspector/V8Inspector.h"
#include <stdio.h>
#include <libplatform/libplatform.h>
#include <v8.h>

using namespace blink;

class ShellArrayBufferAllocator : public v8::ArrayBuffer::Allocator {
 public:
  virtual void* Allocate(size_t length) {
    void* data = AllocateUninitialized(length);
    return data == NULL ? data : memset(data, 0, length);
  }
  virtual void* AllocateUninitialized(size_t length) { return malloc(length); }
  virtual void Free(void* data, size_t) { free(data); }
};

int main(int argc, char* argv[]) {
    v8::V8::InitializeICU();
    v8::Platform* platform = v8::platform::CreateDefaultPlatform();
    v8::V8::InitializePlatform(platform);
    v8::V8::Initialize();
    v8::V8::SetFlagsFromCommandLine(&argc, argv, true);
    ShellArrayBufferAllocator array_buffer_allocator;
    v8::Isolate::CreateParams create_params;
    create_params.array_buffer_allocator = &array_buffer_allocator;
    v8::Isolate* isolate = v8::Isolate::New(create_params);
    run_shell = (argc == 1);

    V8Inspector* agent = new V8Inspector();
    fprintf(stderr, "V8 inspector is running %p.\n", &agent);

    int result;
    {
        v8::Isolate::Scope isolate_scope(isolate);
        v8::HandleScope handle_scope(isolate);
        v8::Handle<v8::Context> context = CreateShellContext(isolate);
        if (context.IsEmpty()) {
            fprintf(stderr, "Error creating context\n");
            return 1;
        }
        v8::Context::Scope context_scope(context);
        result = RunMain(isolate, argc, argv);
        if (run_shell) RunShell(context);
    }
    isolate->Dispose();
    v8::V8::Dispose();
    v8::V8::ShutdownPlatform();
    delete platform;
    return result;
}