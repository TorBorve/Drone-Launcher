#!/bin/bash

if [ -z "$1" ]
  then
    echo "You need to specify the path to the git repository"
    exit
fi

cd $1

GIT_REPO_DIR=$PWD
git fetch
git pull

cd ../..
catkin build drone_launcher_msgs_pkg
source devel/setup.bash

rosrun rosserial_client make_libraries $GIT_REPO_DIR/embedded_sw/lib drone_launcher_msgs_pkg