#!/bin/bash

IP=$(curl -s http://192.168.1.53:8080)
echo -e "\e[94mBuild Number:\e[39m $IP"
