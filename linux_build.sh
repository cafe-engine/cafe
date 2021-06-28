#!/bin/sh

TARGETS="Windows Web Linux"
FILES="cafe libcafe.a"

build()
{
    TARGET=$1
    echo "Copying files for $TARGET target"
    case "$1" in
	Web)
	    FILES="index.html cafe.js cafe.wasm"
	    ;;
	Windows)
	    FILES="cafe.exe libcafe.a SDL2.dll"
	    ;;
	Linux)
	    FILES="cafe libcafe.a"
	    ;;
    esac

    for FILE in $FILES; do
	if [ ! -d build/$TARGET ];
	then
	    mkdir build/$TARGET
	fi
	cp $FILE build/$TARGET
    done
}

mkdir build/
for TARGET in $TARGETS; do
    make TARGET=$TARGET cafe
    build $TARGET
    make TARGET=$TARGET clean
done
