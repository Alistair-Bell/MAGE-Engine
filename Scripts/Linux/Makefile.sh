#! /bin/bash
echo "Running Premake"
./Externals/Linux/premake5 --cc=clang gmake2
echo "Running make"
make
./Build/Binaries/Sandbox-Debug-linux-x86_64/Sandbox