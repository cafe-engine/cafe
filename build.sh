#!/bin/bash

git_base=https://github.com/cafe-engine
submodules="tea mocha latte coffee"
submodules_dir=modules
out=cafe
dir=$(pwd)


setup() {
    #for mod in $submodules; do
    #	git clone $git_base/$mod --recursive
    #done
    git submodule update --init --recursive
}

clean() {
    make clean
    for mod in $submodules; do
        make clean -C $submodules_dir/$mod
    done
}

update-mak() {
    git add Makefile
    git commit -m "update Makefile"
    git push origin main
    for mod in $submodules; do
        cp Makefile $submodules_dir/$mod/Makefile
	cd $submodules_dir/$mod
	git add Makefile
	git commit -m "update Makefile"
	git push origin main
	cd $dir 
    done
}

pull() {
    git pull origin main
    for mod in $submodules; do
        cd $submodules_dir/$mod
	git pull origin main
	cd $dir
    done
}

run() {
    ./bin/$out
}

compile() {
    arg=$1
    if [[ $arg == "" ]]; then
        $arg=.
    fi
    make -C $arg
}

case $1 in
setup) setup ;;
clean) clean ;;
update-mak) update-mak ;;
pull) pull ;;
run) run ;;
*) compile $1 ;;
esac
