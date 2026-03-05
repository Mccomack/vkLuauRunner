#!/bin/bash
set -e

./build.sh "Debug"

xattr -cr build/Debug/program

mkdir -p runtest && cd runtest && lldb ../build/Debug/program -o "process launch --working-dir $(pwd)"