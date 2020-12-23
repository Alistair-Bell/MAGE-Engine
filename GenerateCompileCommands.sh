#!/bin/bash

cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -D PROJECT_BUILD_UNIT_TESTS=On -D CMAKE_BUILD_TYPE=Debug -D PROJECT_BUILD_PLATFORM=Linux -B Build
cp Build/compile_commands.json compile_commands.json


