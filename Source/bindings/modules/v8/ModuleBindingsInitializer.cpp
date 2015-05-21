// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "config.h"
#include "bindings/modules/v8/ModuleBindingsInitializer.h"

#include "bindings/core/v8/ModuleProxy.h"
#include "bindings/core/v8/V8PerIsolateData.h"
#include "bindings/modules/v8/SerializedScriptValueForModulesFactory.h"
#include "core/dom/ExecutionContext.h"

namespace blink {

// initPartialInterfacesInModules is generated by
// generate_init_partial_interfaces.py.
void initPartialInterfacesInModules();

void ModuleBindingsInitializer::init()
{
    initPartialInterfacesInModules();
    SerializedScriptValueFactory::initialize(new SerializedScriptValueForModulesFactory);
}

} // namespace blink
