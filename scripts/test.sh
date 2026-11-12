#!/bin/bash
set -e

./scripts/build.sh "Debug" test

ctest --test-dir build/Debug -j8 --output-on-failure "$@"
