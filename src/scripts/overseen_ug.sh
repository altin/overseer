#!/bin/bash
GROUP=$(id -ng)
DHCLIENT=$(head -n 1 /etc/overseer/src/scripts/overseer_dhpath)
DHPATH=$(dirname $DHCLIENT)
DHCLIENTOLD="$DHPATH/overseer_dhclient_sudo.conf"
OPENDNS="supersede domain-name-servers 208.67.222.123,208.67.220.123;"

if [ $GROUP != "overseen" ]; then
    if grep -q "$OPENDNS" $DHCLIENT; then
        cp -f $DHCLIENTOLD $DHCLIENT
        echo "OpenDNS is now inactive"
    fi
fi

if [ $GROUP = "overseen" ]; then
	echo "Overseen user logged in"
    cd /etc/overseer/src/pcontrol
	#setsid ./pcontrol
fi
