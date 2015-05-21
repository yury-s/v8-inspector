// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "config.h"

#include "public/web/WebDevToolsAgent.h"
#include "web/WebDevToolsAgentImpl.h"
#include <stdio.h>


int main()
{
    blink::WebDevToolsAgentImpl* agent = nullptr;
    fprintf(stderr, "V8 inspector is running %p.\n", &agent);
    return 0;
}