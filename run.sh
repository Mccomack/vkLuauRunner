#!/bin/bash
set -e

./build.sh "Release"

mkdir -p runtest && cd runtest && ../build/Release/program