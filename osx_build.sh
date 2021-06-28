#!/bin/sh

# Uses a Linux machine to make other builds
# i use a basic Debian vm

LM_HOST=127.0.0.1
LM_USER=c4nx1
LM_PORT=2522

if [ ! -d build ];
then
    mkdir build/
fi

ssh -p $LM_PORT -t $LM_USER@$LM_HOST 'sudo rm -r ~/.tmp/cafe'
scp -P $LM_PORT -r ../cafe $LM_USER@$LM_HOST:~/.tmp
ssh -p $LM_PORT -t $LM_USER@$LM_HOST 'cd ~/.tmp/cafe && bash -ic "./linux_build.sh"'
scp -P $LM_PORT -r $LM_USER@$LM_HOST:"~/.tmp/cafe/build/*" build/

mkdir build/OSX
make TARGET=OSX
cp cafe build/OSX/
cp libcafe.a build/OSX/
make TARGET=OSX clean
