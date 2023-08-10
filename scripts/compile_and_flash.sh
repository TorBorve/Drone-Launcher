#!/bin/bash

if [ -z "$1" ]
  then
    echo "You need to specify the path to the git repository"
    exit
fi

cd $1

cd embedded_sw/
pio run

printf "\n\nFlashing code to teensy. Press reset button"
./../build/teensy_loader_cli/teensy_loader_cli --mcu=TEENSY41 -w -v .pio/build/teensy41/firmware.hex