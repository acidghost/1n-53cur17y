#!/usr/bin/env bash

source settings.sh

set -x
scp ./Makefile ./hijack.c ${user}@${ip}:${folder}/
ssh ${user}@${ip} 'cd '${folder}' && make clean all'
