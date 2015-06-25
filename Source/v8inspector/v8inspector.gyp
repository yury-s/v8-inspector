# Copyright 2015 The Chromium Authors. All rights reserved.
# Use of this source code is governed by a BSD-style license that can be
# found in the LICENSE file.

{
    'includes': [
        '../wtf/wtf.gypi',
    ],
    'targets': [
        {
            'target_name': 'iojs_inspector',
            'type': 'executable',
            'dependencies': [
                '../config.gyp:config',
                '../core/core.gyp:webcore_v8inspector',
                '../wtf/wtf.gyp:wtf',
#                '../chrome/base/base.gyp:base',
                '../io.js/deps/v8/tools/gyp/v8.gyp:v8',
                '../io.js/deps/v8/tools/gyp/v8.gyp:v8_libplatform', # for V8InspectorMain

                '../io.js/node.gyp:iojs',
            ],
            'sources': [
                'V8Inspector.cpp',
                'V8Inspector.h',
            ],
            'include_dirs': [
                '..',  # WebKit/Source
                #'../chrome',  # WebKit/Source/chrome
                '../io.js/deps/v8', # for include/v8-platform.h in include/libplatform/libplatform.h
            ],
            'defines': [
                'INSIDE_BLINK',
            ],
        },
    ],
}
