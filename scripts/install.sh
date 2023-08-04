#!/bin/bash

if [ -z "$1" ]
  then
    echo "You need to specify the path to the build folder"
    exit
fi

cd $1

BUILD_DIR=$PWD

printf "Installing rosserial"
sudo apt install python3-catkin-tools ros-melodic-rosserial-python ros-melodic-rosserial-client tar -y

printf "Installing platformio core"
sudo apt install python3 curl python3-venv -y
python3 -c "$(curl -fsSL https://raw.githubusercontent.com/platformio/platformio/master/scripts/get-platformio.py)"
echo "export PATH=\$PATH:/home/${USER}/.platformio/penv/bin" >> ~/.bashrc


printf "Installing teensy_loader_cli"
sudo apt install libusb-dev -y
git clone https://github.com/PaulStoffregen/teensy_loader_cli.git
cd teensy_loader_cli
make
cd $BUILD_DIR

printf "Installing TeensyDuino"
wget https://downloads.arduino.cc/arduino-1.8.15-linuxaarch64.tar.xz
wget https://www.pjrc.com/teensy/td_154/TeensyduinoInstall.linuxaarch64
wget https://www.pjrc.com/teensy/00-teensy.rules
sudo cp 00-teensy.rules /etc/udev/rules.d/
tar -xf arduino-1.8.15-linuxaarch64.tar.xz
chmod 755 TeensyduinoInstall.linuxaarch64
./TeensyduinoInstall.linuxaarch64 --dir=arduino-1.8.15
cd arduino-1.8.15/hardware/teensy/avr/cores/teensy4
make
