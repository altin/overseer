#!/bin/bash
set -e

ARG1=$1
log="log.txt"

if [ $ARG1 == "install" ]; then
    cd src/pcontrol
    make -f Makefile
    cd ..
    make -f Makefile
    ./overseer install
elif [ $ARG1 == "uninstall" ]; then
    cd src
    ./overseer uninstall
    make clean -f Makefile
    cd pcontrol
    make clean -f Makefile
else
    echo "Error: Incorrect argument. Usage: ./run [install | uninstall]"
fi