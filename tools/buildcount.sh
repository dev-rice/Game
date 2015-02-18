#!/bin/bash

NUM=$(curl -X POST -s http://yakkio.com:8080)
printf "Build Number: \033[94m%s\033[39m\n" "$NUM"