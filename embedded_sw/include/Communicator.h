#pragma once

#include <stdint.h>
#include <ros.h>
#include <std_msgs/UInt8.h>

#include <TeensyThreads.h>

#include "Leds.h"


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

    ros::Publisher _statusPub;
    ros::Subscriber<std_msgs::UInt8> _fireSub;
    ros::Subscriber<std_msgs::UInt8> _loadSub;
    ros::Subscriber<std_msgs::UInt8> _unloadSub;
    std_msgs::UInt8 _statusMsg;
    bool _init;
    uint32_t _prevUpdate;
    Led _aliveLed;
};

extern Communicator communicator;