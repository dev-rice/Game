#!/bin/bash

IP=$(curl -s http://192.168.1.53:8080)
printf "\e[94m%s\e[39m %s\n" "Build Number:" "$IP"
