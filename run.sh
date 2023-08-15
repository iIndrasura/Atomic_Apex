#!/bin/bash

if [ -f benzenee ]; then
# if [ -f "build/benzenee/benzenee" ]; then
    filename=$(openssl rand -hex $((RANDOM % 11 + 10)))
    cp benzenee "$filename"
    # cp "build/benzenee/benzenee" "$filename"
    sudo chmod 700 "$filename"
    sudo ./"$filename"
    rm "$filename"
fi