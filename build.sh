#!/bin/bash

./Source/chrome/build/gyp_chromium --depth ./Source/chrome/  -Dv8_use_external_startup_data=0 ./Source/v8inspector/v8inspector.gyp -G output_dir=$(pwd)/out
