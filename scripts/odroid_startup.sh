#!/bin/bash

#This script should be run when the odroid starts

sleep 10

SCRIPT_DIR="${0%/*}/"

cd $SCRIPT_DIR

source ../../../devel/setup.bash

bash connect_to_teensy.sh