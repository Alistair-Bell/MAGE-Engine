#!/bin/bash
clear

if [ "$#" -le 0 ]; then
    buildMode=Debug # automaticly use debug
else
    buildMode=$1
fi

cmake -B Build -D PROJECT_BUILD_PLATFORM=Linux -D CMAKE_BUILD_TYPE=$buildMode -D PROJECT_BUILD_UNIT_TESTS=On

cd Build
make
make test
cd ../
./Build/Sandbox/Sandbox
