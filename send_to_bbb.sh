#!/usr/bin/bash
# A script to copy a file to Beaglebone Black through SSH

if [ $# -eq 0 ]; then
echo "Usage: bash send_to_bbb.sh [file]"
else
scp ./$1 debian@192.168.6.2:~
fi
