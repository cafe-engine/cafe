#!/bin/sh

TARGETS="Windows Web Linux"

mkdir build/
for TARGET in $TARGETS; do
    mkdir build/$TARGET
    make TARGET=$TARGET cafe
    cp cafe* build/$TARGET
    cp libcafe* build/$TARGET
    make TARGET=$TARGET clean
done
