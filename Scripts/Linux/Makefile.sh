#! /bin/bash
clear
echo "Running Premake"
./Externals/Linux/premake5 gmake2 -cc=gcc
echo "Running make"
make config="$@" 
./Build/Binaries/MageEngine/MageEngine

