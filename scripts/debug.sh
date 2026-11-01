#!/bin/bash
set -e

./scripts/build.sh "Debug"

xattr -cr build/Debug/src/program

mkdir -p runtest && cd runtest && lldb ../build/Debug/src/program -o "process launch --working-dir $(pwd)"
