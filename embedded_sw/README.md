# Drone Launcher Code

# Description

The Majority of the software is running on the Teensy. The Odroid is mainly used for communication over the Rajant Radios. The Sowtware running on the micro controller is divided into these main modules:

- **Communicator**

    This module is responsible for the communication with the odroid over rosserial. It sends status messages to the odroid and recieves commands sent over ROS. 

- **Menu**

    This module controls the LCD display and handles the logic for the menu. If the user selects an action on the menu the module will forward this information to the Launch System module.

- **Launch System**

    This module controls all the Launch Units (all "chambers"). The module contains several instances of the Launch Unit class and calls the relevant fire, load or unload commands when the user calls these commmands.

- **Battery Monitor**

    This is a small Module that reads the voltage from the battery and sends this information to the communicator.

- **Navagator**
    This module is responsible for giving informatio about the location and orientation of the launcher. This is done by reading the data from a GPS and a IMU. 

All of these modules has their own init functio that needs to be called at the beginning of the program as well as a update function that should be run periodically. 