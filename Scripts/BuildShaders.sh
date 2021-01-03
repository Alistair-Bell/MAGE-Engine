#!/bin/bash

files=$(find -type f -regex ".*/.*\.\(vert\|frag\)")

for i in $files; do
    echo "$i.sprv"
    result=$(glslc $i -o $i.sprv)
    if [ $? == 0 ]; then
        echo "Succesfully compiled $i"
    else
        echo "Failed to compile $i"
    fi
done

