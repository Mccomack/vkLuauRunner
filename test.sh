#!/bin/bash
set -e

./build.sh "Debug" test

ctest --test-dir build/Debug --output-on-failure "$@"