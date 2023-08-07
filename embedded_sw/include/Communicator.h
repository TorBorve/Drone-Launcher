#pragma once

#include <TeensyThreads.h>
#include <ros.h>
#include <std_msgs/UInt8.h>
#include <stdint.h>

#include "LaunchSystem.h"
#include "Leds.h"
#include "drone_launcher_pkg/LauncherStatus.h"

namespace DroneLauncher {

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
    static int8_t toMsg(LaunchUnit::State state);
    void updateStatus(uint32_t now);

    ros::Publisher _statusPub;
    ros::Subscriber<std_msgs::UInt8> _fireSub;
    ros::Subscriber<std_msgs::UInt8> _loadSub;
    ros::Subscriber<std_msgs::UInt8> _unloadSub;
    drone_launcher_pkg::LauncherStatus _statusMsg;
    drone_launcher_pkg::LaunchUnitStatus _luStatusMsgs[LS_NUM_UNITS];
    bool _init;
    Led _aliveLed;
    uint32_t _prevUpdate;
    uint32_t _prevStatusUpdate;
};

extern Communicator communicator;

}  // namespace DroneLauncher