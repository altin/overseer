#!/bin/bash

if [ `id -ng` = "overseen" ]; then
    touch $HOME/file
    echo "Created file in home"
fi
