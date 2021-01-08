#!/bin/bash

function StoreInLocation()
{
    path=$1/MageEngine
    if [ -d $path ]; then
        echo "Directory already exists, just copying files"
    else
        echo "Making MageEngine directory in $1 (directory empty), copying required resource files"
        mkdir $path
    fi

}

if [ "$#" -le 0 ]; then
    buidMode="Debug"
else
    buidMode=$1
fi

clear
# Compilation of a series of scripts for the lazy amoung us
echo "MageEngine automated build system for minimal effort"

echo "Building all the required shaders"
./Scripts/BuildShaders.sh

# Calls cmake then make, assumes using make build system as this is linux afterall
echo "Generating project build files"
./Scripts/GenerateAndCompile.sh $buidMode # Creates compile_commands.json for coc

echo "Generating intelisense files for development"
./Scripts/GenerateCompileCommands.sh # Defaults to build mode debug, development should use debug environment

# Copies all needed resources to a set directory if the user desires, default choice
installResourceDir=(~/.local/MageEngine)
echo "Installing common resources to $installResourceDir"

if [ ! -d $installResourceDir ]; then
    mkdir $installResourceDir
fi

cp -r SharedResources $installResourceDir
