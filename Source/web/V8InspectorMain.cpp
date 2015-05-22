// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
#include "config.h"

#include "web/V8Inspector.h"
#include <stdio.h>

using namespace blink;

int main()
{
    V8Inspector* agent = new V8Inspector(nullptr);
    fprintf(stderr, "V8 inspector is running %p.\n", &agent);
    return 0;
}