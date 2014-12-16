#!/bin/bash

IP=$(curl -s http://yakkio.com:8080)
printf "\e[94m%s\e[39m %s\n" "Build Number:" "$IP"
