#!/bin/bash
set -e

ARG1=$1

if [ $ARG1 == "install" ]; then
	mkdir /etc/overseer
	cp -rp src /etc/overseer
    cd /etc/overseer/src
    make -f Makefile
    ./overseer install
elif [ $ARG1 == "uninstall" ]; then
    cd /etc/overseer/src
    ./overseer uninstall
    make clean -f Makefile
	cd /etc
	rm -r overseer
else
    echo "Error: Incorrect argument. Usage: sudo ./run.sh [install | uninstall]"
fi
