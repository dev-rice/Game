#!/bin/bash

# Check if you are on the local network
# If you are, then set the host to yakkio's local
# ip, otherwise use the domain name.
IP=$(curl -s icanhazip.com)
LOCAL_IP="76.120.23.142"
if [ "$IP" ==  "$LOCAL_IP" ]
    then
    HOST="192.168.1.53"
else
    HOST="yakkio.com"
fi

NUM=$(curl -X POST -s http://$HOST:8080)
printf "Build Number: \033[94m%s\033[39m\n" "$NUM"
