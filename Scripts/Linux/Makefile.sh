#! /bin/bash
clear
echo "Running Premake"
./Externals/Linux/premake5 gmake2 -cc=glang 
echo "Running make"
make config="$@" 
./Build/Binaries/MageEngine/MageEngine

