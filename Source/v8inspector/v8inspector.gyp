# Copyright 2015 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
    'includes': [
        '../bindings/bindings.gypi',
        '../build/features.gypi',
        '../build/scripts/scripts.gypi',
        '../core/core.gypi',
        '../platform/blink_platform.gypi',
        '../wtf/wtf.gypi',
    ],
    'targets': [
        {
            'target_name': 'v8inspector_main',
            'type': 'executable',
            'dependencies': [
                '../config.gyp:config',
                '../platform/blink_platform.gyp:blink_platform',
                '../core/core.gyp:webcore',
                '../core/core.gyp:webcore_generated',
            ],
            'sources': [
                'V8InspectorMain.cpp',
                'V8Inspector.cpp',
                'V8Inspector.h',
            ],
            'include_dirs': [
                '../',
            ],
            'defines': [
                'BLINK_IMPLEMENTATION=1',
                'INSIDE_BLINK',
            ],
        },

    ],
}
