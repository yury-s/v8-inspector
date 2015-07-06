V8 inspector backend
====================

This is an experiment with intent to add debugging capabilities to V8.
The UI part is supposed to be regular Chrome DevTools talking to the
v8 inspector via the remote debugging protocol.

All of the code was forked from Chromium r330774 (be901d619720e6d9f23e1585a5039079877e9d30),
Blink r195622(1d565c27217eeadb958cf702dc526f4b06bf8447) and V8 r4.5.7(73ef6572a4e79f51f4591d3f76b61a8d1ac75ba5).

## Build & run

Prerequisites - see Chromium prerequisites. This is all Linux-only at the moment. At the root of the repository run the following scrip:

```text
$ ./tools/gyp_v8inspector.py
$ ninja -C out/Release/ v8inspector_main -j100
```

Run script with remote debugging enabled:

```text
$ ./out/Release/v8inspector_main test.js
```

The command will execute the script passed as the argument and start to wait for remote debugger
connection on port 2015. To connect with DevTools, run Chromium and navigate it to the following URL:

```text
chrome-devtools://devtools/bundled/inspector.html?ws=localhost:2015
```

DevTools UI will load in the tab and connect to the running V8 instance.
