#!/bin/bash

if [ -f Atomic ]; then
# if [ -f "build/Atomic/Atomic" ]; then
    filename=$(openssl rand -hex $((RANDOM % 11 + 10)))
    cp Atomic "$filename"
    # cp "build/Atomic/Atomic" "$filename"
    sudo chmod 700 "$filename"
    sudo ./"$filename"
    rm "$filename"
fi