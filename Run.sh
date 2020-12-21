#!/bin/bash
clear
cmake -B Build -D PROJECT_BUILD_PLATFORM=Linux -D CMAKE_BUILD_TYPE=Debug -D PROJECT_BUILD_UNIT_TESTS=On
cd Build
make
make test
cd ../
./Build/Sandbox/Sandbox
