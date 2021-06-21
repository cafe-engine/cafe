#!/bin/sh

# Uses a Linux machine to make other builds
# i use a basic Debian vm

LM_HOST=127.0.0.1
LM_USER=c4nx1
LM_PORT=2522

scp -P $LM_PORT -r ../cafe $LM_USER@$LM_HOST:~/.tmp
ssh -p $LM_PORT $LM_USER@$LM_HOST "(cd ~/.tmp/cafe && ./linux_build.sh)"
scp -P $LM_PORT -r $LM_USER@$LM_HOST:~/.tmp/cafe/build/ build/

mkdir build/
mkdir build/OSX
make TARGET=OSX
cp cafe* build/OSX
cp libcafe* build/OSX
make TARGET=OSX clean
