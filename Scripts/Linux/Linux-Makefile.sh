#! /bin/bash
echo "Running Premake"
./Externals/premake5 --cc=clang gmake2
echo "Running make"
make