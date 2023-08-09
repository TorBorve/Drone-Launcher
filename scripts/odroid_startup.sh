#!/bin/bash

#This script should be run when the odroid starts

echo "Sleeping for some seconds"
sleep 10

SCRIPT_DIR="${0%/*}/"

cd $SCRIPT_DIR

source ../../../devel/setup.bash

MY_IP=$(hostname -I)
MY_IP=$(echo $MY_IP | sed 's/ //g')

export ROS_MASTER_URI="http://${MY_IP}:11311"
export ROS_IP=$MY_IP


echo "ROS_IP: $ROS_IP"

roslaunch drone_launcher_pkg drone_launcher.launch