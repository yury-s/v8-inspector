# Copyright 2015 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
    'includes': [
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
                '../core/core.gyp:webcore_v8inspector',
                '../wtf/wtf.gyp:wtf',
            ],
            'sources': [
                'V8InspectorMain.cpp',
                'V8Inspector.cpp',
                'V8Inspector.h',
            ],
            'include_dirs': [
                '..',  # WebKit/Source
                '<(SHARED_INTERMEDIATE_DIR)/blink',  # gen/blink for InspectorBackendDispatcher
            ],
            'defines': [
                'INSIDE_BLINK',
            ],
        },

    ],
}
