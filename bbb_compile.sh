#!/usr/bin/bash
# A script to compile a C source code file
# and copy it to Beaglebone Black through SSH

if [ $# -eq 0 ]; then
echo "Usage: bash bbb_compile.sh [output file] [source file]"
else
arm-linux-gnueabihf-gcc -static -o $1 $2
scp ./$1 debian@192.168.6.2:~
fi
