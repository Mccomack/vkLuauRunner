#!/bin/bash
rm -rf build ; mkdir build ; cd build

buildType=""
tmp=$(echo $1 | tr '[:upper:]' '[:lower:]')

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

cmake .. -G "Ninja" -DCMAKE_BUILD_TYPE=$buildType

cmake --build .

cd ..