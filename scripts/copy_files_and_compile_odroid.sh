#!/bin/bash
set -e

SCRIPT_DIR="${0%/*}/"

cd $SCRIPT_DIR

cd ../..

ssh odroid@192.168.12.168 << END
  mkdir -p catkin_ws/src
  cd catkin_ws/src
  rm -rf Drone-Launcher
END

sftp -o PasswordAuthentication=yes odroid@192.168.12.168 <<END
cd catkin_ws/src
put -r Drone-Launcher/
END

ssh odroid@192.168.12.168 << END
  source /opt/ros/melodic/setup.bash
  cd catkin_ws
  catkin clean -y
  catkin build
END
