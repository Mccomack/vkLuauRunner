#!/bin/bash
set -e

./scripts/build.sh "Release" off bundle

mkdir -p runtest && cd runtest

case "$(uname -s)" in
    Darwin) open ../build/Release/program.app ;;
    *) ../build/Release/program ;;
esac
