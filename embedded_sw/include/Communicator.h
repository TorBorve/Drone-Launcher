#pragma once

#include <TeensyThreads.h>
#include <ros.h>
#include <std_msgs/UInt8.h>
#include <stdint.h>

#include "LaunchSystem.h"
#include "Leds.h"
#include "drone_launcher_pkg/LauncherStatus.h"
#include "drone_launcher_pkg/ManualControl.h"

namespace DroneLauncher {

// Class for communicating with ROS
class Communicator {
   public:
    Communicator();
    void init();
    void update(uint32_t now);
    ros::NodeHandle nh;
    mutable Threads::Mutex mutex;

   private:
    static void fireCallback(const std_msgs::UInt8& msg);
    static void loadCallback(const std_msgs::UInt8& msg);
    static void unloadCallback(const std_msgs::UInt8& msg);
    static void manualControlCallback(const drone_launcher_pkg::ManualControl& msg);
    static int8_t toMsg(LaunchUnit::State state);
    void updateStatus();

    ros::Publisher _statusPub;
    ros::Subscriber<std_msgs::UInt8> _fireSub;
    ros::Subscriber<std_msgs::UInt8> _loadSub;
    ros::Subscriber<std_msgs::UInt8> _unloadSub;
    ros::Subscriber<drone_launcher_pkg::ManualControl> _manualControlSub;
    drone_launcher_pkg::LauncherStatus _statusMsg;
    drone_launcher_pkg::LaunchUnitStatus _luStatusMsgs[LS_NUM_UNITS];
    bool _init;
    Led _aliveLed;
    uint32_t _prevUpdate;
    uint32_t _prevStatusUpdate;
};

extern Communicator communicator;

}  // namespace DroneLauncher