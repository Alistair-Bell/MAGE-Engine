#!/bin/bash
cmake -B Build -D CMAKE_EXPORT_COMPILE_COMMANDS=1 -D PROJECT_BUILD_PLATFORM=Linux -D PROJECT_BUILD_LINUX_DISPLAY_BACKEND=X11 -D CMAKE_BUILD_TYPE=$1 -D PROJECT_BUILD_UNIT_TESTS=On

# Go into build and then make
cd Build
cp compile_commands.json ../

# Engine
make

# For unit tests
make test

# Exit directoty
cd ../
