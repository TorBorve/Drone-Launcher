#!/bin/bash

MY_IP=$(hostname -I)
MY_IP=$(echo $MY_IP | sed 's/ //g')

export ROS_MASTER_URI="http://${MY_IP}:11311"
export ROS_IP=$MY_IP


echo "ROS_IP: $ROS_IP"


while true
do
roscore & sleep 10; rosrun rosserial_python serial_node.py /dev/ttyACM0 && fg
    sleep 10
done
