#!/bin/bash
set -e

buildType=""
tmp=$(echo "$1" | tr '[:upper:]' '[:lower:]')

if [ "$tmp" == "release" ]; then
    buildType="Release"
else
    if [ "$tmp" != "debug" ] 
    then
        echo "Please set build type(Release, Debug) at the first argument. using default build type(Debug)..."
    fi

    buildType="Debug"
fi

buildTest="OFF"
tmp=$(echo "$2" | tr '[:upper:]' '[:lower:]')
if [ "$tmp" == "test" ]; then
    buildTest="ON"
fi

case "$(uname -s)" in
    Darwin) target="$HOME/Library/Application Support/vkLuauRunner" toolchain="cmake/toolchain/macos.cmake" ;;
    *)      target="$HOME/.local/share/vkLuauRunner" toolchain="cmake/toolchain/linux.cmake";;
esac

cmake -S . -B "build/$buildType" -G "Ninja" -DCMAKE_TOOLCHAIN_FILE=$toolchain -DCMAKE_BUILD_TYPE=$buildType -DBUILD_TEST=$buildTest

cmake --build "build/$buildType"

ln -sfn "$buildType" "build/current"

mkdir -p logs
mkdir -p runtest

ln -sfn ../build/shaders runtest/shaders
ln -sfn ../asset runtest/asset
ln -sfn ../logs runtest/logs

ln -sfn "$(pwd)/runtest" "$target"