#!/usr/bin/env python
import glob
import os
import shlex
import sys

script_dir = os.path.dirname(__file__)
v8inspector_root = os.path.normpath(os.path.join(script_dir, os.pardir))
chrome_root = os.path.join(os.path.abspath(v8inspector_root), 'Source', 'chrome')

sys.path.insert(0, os.path.join(chrome_root, 'tools', 'gyp', 'pylib'))

import gyp

# Add paths so that pymod_do_main(...) can import files.
sys.path.insert(1, os.path.join(chrome_root, 'build'))

# Directory within which we want all generated files (including Makefiles)
# to be written.
output_dir = os.path.join(os.path.abspath(v8inspector_root), 'out')

def run_gyp(args):
  rc = gyp.main(args)
  if rc != 0:
    print 'Error running GYP'
    sys.exit(rc)

if __name__ == '__main__':
  args = sys.argv[1:]

  args.extend(['-I', os.path.join(chrome_root, 'build', 'common.gypi')])

  args.append('--depth=' + chrome_root)

  # Tell gyp to write the Makefiles into output_dir
  args.extend(['--generator-output', output_dir])

  # Tell make to write its output into the same dir
  args.extend(['-Goutput_dir=' + output_dir])

  args.append('-Dv8_use_external_startup_data=0')
  args.append('-Duse_goma=1')
  args.append(os.path.join(v8inspector_root, 'Source', 'v8inspector', 'v8inspector.gyp'))

  gyp_args = list(args)
  print "gyp_args = " + str(gyp_args)
  run_gyp(gyp_args)
