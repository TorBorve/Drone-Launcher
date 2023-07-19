#include "Communicator.h"

#include "LaunchSystem.h"
#include "Log.h"
#include "Pins.h"

#define COM_UPDATE_INTERVAL 100

Communicator communicator{};

Communicator::Communicator() : nh{},
                               _statusPub{"drone_launcher/status", &_statusMsg},
                               _fireSub{"drone_launcher/fire", &fireCallback},
                               _loadSub{"drone_launcher/load", &loadCallback},
                               _unloadSub{"drone_launcher/unload", &unloadCallback},
                               _init{false},
                               _prevUpdate{0},
                               _aliveLed{PIN_LED_ALIVE, BaseLed::Mode::BLINK} {}

void Communicator::init() {
    if (!_init) {
        {
            Threads::Scope lock{mutex};
            nh.initNode();
            nh.advertise(_statusPub);
            nh.subscribe(_fireSub);
            nh.subscribe(_loadSub);
            nh.subscribe(_unloadSub);
        }
        _aliveLed.init();
        _init = true;
        LOG_INFO("Communicator initialized");
    }
}

void Communicator::update(uint32_t now) {
    if (_init && now - _prevUpdate > COM_UPDATE_INTERVAL) {
        _prevUpdate = now;
        _statusMsg.data = 0;
        _statusPub.publish(&_statusMsg);
        nh.spinOnce();  // Not thread safe since nh is used in logging.
        _aliveLed.update(now);
    }
}

void Communicator::fireCallback(const std_msgs::UInt8& msg) {
    LOG_INFO("Received fire message");
    launchSystem.fire(msg.data);
}

void Communicator::loadCallback(const std_msgs::UInt8& msg) {
    LOG_INFO("Received load message");
    launchSystem.load(msg.data, 0);
}

void Communicator::unloadCallback(const std_msgs::UInt8& msg) {
    LOG_INFO("Received unload message");
    launchSystem.unload(msg.data);
}
