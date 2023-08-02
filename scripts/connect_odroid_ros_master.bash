# Odroid is running a ROS master. This should be at the ip address: 192.168.12.168

ODROID_IP=192.168.12.168
YOUR_IP=192.168.12.69

export ROS_MASTER_URI=http://$ODROID_IP:11311
export ROS_IP=$YOUR_IP