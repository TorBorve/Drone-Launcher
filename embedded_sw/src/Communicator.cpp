#include "Communicator.h"

#include "LaunchSystem.h"
#include "Pins.h"

#define COM_UPDATE_INTERVAL 100

Communicator communicator{};

Communicator::Communicator() : _nh{},
                               _statusPub{"status", &_statusMsg},
                               _fireSub{"fire", &fireCallback},
                               _loadSub{"load", &loadCallback},
                               _unloadSub{"unload", &unloadCallback},
                               _init{false},
                               _prevUpdate{0},
                               _aliveLed{PIN_LED_ALIVE, BaseLed::Mode::BLINK} {}

void Communicator::init() {
    if (!_init) {
        _nh.initNode();
        _nh.advertise(_statusPub);
        _nh.subscribe(_fireSub);
        _nh.subscribe(_loadSub);
        _nh.subscribe(_unloadSub);
        _aliveLed.init();
        _init = true;
    }
}

void Communicator::update(uint32_t now) {
    if (_init && now - _prevUpdate > COM_UPDATE_INTERVAL) {
        _nh.loginfo("Communicator update");
        _prevUpdate = now;
        _statusMsg.data = 0;
        _statusPub.publish(&_statusMsg);
        _nh.spinOnce();
        _aliveLed.update(now);
    }
}

void Communicator::fireCallback(const std_msgs::UInt8& msg) {
    communicator._nh.loginfo("Fire callback");
    launchSystem.fire(msg.data);
}

void Communicator::loadCallback(const std_msgs::UInt8& msg) {
    communicator._nh.loginfo("Load callback");
    launchSystem.load(msg.data, 0);
}

void Communicator::unloadCallback(const std_msgs::UInt8& msg) {
    communicator._nh.loginfo("Unload callback");
    launchSystem.unload(msg.data);
}
