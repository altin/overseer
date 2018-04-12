#!/bin/bash
GROUP=$(id -ng)
DHPATH=$(find /etc -name dhclient.conf)
OPENDNS="supersede domain-name-server 208.67.222.123,208.67.220.123;"

if [ $GROUP != "overseen" ]; then
    sudo cp -f .overseen_on_logout $HOME/.bash_logout
    if grep -q $OPENDNS $DHPATH; then
        sed "s/$OPENDNS/''/g" $DHPATH > $DHPATH
        sudo systemctl restart systemd-resolved.service
        sudo service network-manager restart
    fi
fi

if [ $GROUP = "overseen" ]; then
    touch $HOME/file
    echo "Created file in home"
fi