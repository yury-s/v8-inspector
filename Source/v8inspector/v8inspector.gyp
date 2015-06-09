# Copyright 2015 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
    'includes': [
        '../wtf/wtf.gypi',
    ],
    'targets': [
        {
            'target_name': 'v8inspector_main',
            'type': 'executable',
            'dependencies': [
                '../config.gyp:config',
                '../core/core.gyp:webcore_v8inspector',
                '../wtf/wtf.gyp:wtf',
                '<(DEPTH)/v8/tools/gyp/v8.gyp:v8',
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
