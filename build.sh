#!/bin/bash

BUILD_TYPE="Debug"

if [ "$1" == "Release" ]; then
	BUILD_TYPE="Release"
elif [ "$1" == "Debug" ]; then
	BUILD_TYPE="Debug"
fi

mkdir -p build
cd build
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..
make
cp OpenglTemplate ..
