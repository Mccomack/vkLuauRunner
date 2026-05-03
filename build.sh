#!/bin/bash
set -e

buildType=""
tmp=$(echo "$1" | tr '[:upper:]' '[:lower:]')

if [ "$tmp" == "release" ] 
then
    buildType="Release"
else
    if [ "$tmp" != "debug" ] 
    then
        echo "Please set build type(Release, Debug) at the first argument. using default build type(Debug)..."
    fi

    buildType="Debug"
fi

cmake -S . -B "build/$buildType" -G "Ninja" -DCMAKE_BUILD_TYPE=$buildType

cmake --build "build/$buildType"

mkdir -p runtest

ln -sfn "$buildType" "build/current"

ln -sfn ../build/shaders runtest/shaders
ln -sfn ../asset runtest/asset

case "$(uname -s)" in
    Darwin) target="$HOME/Library/Application Support/vkLuauRunner" ;;
    *)      target="$HOME/.local/share/vkLuauRunner" ;;
esac

ln -sfn "$(pwd)/runtest" "$target"