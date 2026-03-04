#!/bin/bash
set -e

./build.sh "Debug"

mkdir -p runtest && cd runtest && lldb ../build/Debug/program