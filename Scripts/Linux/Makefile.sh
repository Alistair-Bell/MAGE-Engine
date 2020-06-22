#! /bin/bash
clear
echo "Running Premake"
./Externals/Linux/premake5 --dotnet=mono gmake2 -cc=glang gmake2
echo "Running make"
make config="$@" 
./Build/Binaries/MageEngine/MageEngine

