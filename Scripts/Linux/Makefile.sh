#! /bin/bash
clear

echo "Running Premake"
./Externals/Linux/premake5 --cc=clang gmake2
echo "Running make"
make config="$1" 

if [ "$1" == "debug" ]
then 
    ./Build/Binaries/MageEngine-Debug/MageEngine
else
    ./Build/Binaries/MageEngine-Release/MageEngine
fi

