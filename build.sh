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

ln -sfn "$buildType" "build/current"