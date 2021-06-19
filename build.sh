#!/bin/sh

TARGETS="Windows Web Linux"

mkdir build/
for TARGET in $TARGETS; do
    mkdir build/$TARGET
    make PLATFORM=$TARGET 
    cp cafe* build/$TARGET
    cp libcafe* build/$TARGET
    make PLATFORM=$TARGET clean
done
