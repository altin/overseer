#!/bin/bash
# Overseer logout script

OPENDNS="supersede domain-name-servers 208.67.222.123,208.67.220.123;"
DHCLIENT=$(head -n 1 /etc/overseer_dhpath)
DHPATH=$(dirname $DHCLIENT)
DHCLIENTOVERSEEN="$DHPATH/overseer_dhclient_overseen.conf"

if grep -q "$OPENDNS" $DHPATH; then
	echo "OpenDNS already installed"
else
	cp -f $DHCLIENTOVERSEEN $DHCLIENT
	echo "OpenDNS is now active"
	sudo systemctl restart systemd-resolved.service
	sudo service network-manager restart
fi

