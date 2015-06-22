#!/bin/bash

./Source/chrome/build/gyp_chromium --depth ./Source/chrome/  -Dv8_use_external_startup_data=0 ./Source/v8inspector/v8inspector.gyp -G output_dir=$(pwd)/out
<<<<<<< HEAD

# Build with Goma
ninja -C out/Release/ v8inspector_main -j1000
=======
>>>>>>> 1e0bff693debb78150069dbbce19442609fd8db4
