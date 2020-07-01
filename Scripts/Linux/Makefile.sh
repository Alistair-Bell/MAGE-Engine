#! /bin/bash
clear
echo "Running Premake"
./Externals/Linux/premake5 --cc=clang gmake2
echo "Running make"
make config="$@" 
./Build/Binaries/MageEngine/MageEngine

