#!/bin/bash
set -e

./scripts/build.sh "Debug" test

ctest --test-dir build/Debug --output-on-failure "$@"
