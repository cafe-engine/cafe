#!/bin/bash

git_base=https://github.com/cafe-engine
submodules="tea mocha latte coffee"
submodules_dir=modules
out=cafe


setup() {
	#for mod in $submodules; do
	#	git clone $git_base/$mod --recursive
	#done
        git submodule update --init --recursive
}

clean() {
	for mod in $submodules; do
		make clean -C $submodules_dir/$mod
	done
}

update-mak() {
	for mod in $submodules; do
		cp Makefile $submodules_dir/$mod/Makefile
		cd $mod
		git add Makefile
		git commit -m "update Makefile"
		git push origin main
		cd ..
	done
}

pull() {
	for mod in $submodules; do
		cd $mod
		git pull origin main
		cd ..
	done
}

run() {
	./bin/$out
}

compile() {
	make -C $1
}

case $1 in
setup) setup ;;
clean) clean ;;
update-mak) update-mak ;;
pull) pull ;;
run) run ;;
*) compile . ;;
esac
