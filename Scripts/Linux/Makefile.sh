#! /bin/bash
clear
echo "Running Premake"
./Externals/Linux/premake5 --dotnet=mono gmake2 -cc=clang gmake2
echo "Running make"
make config="$1" 
./Build/Binaries/MageEngine/MageEngine

