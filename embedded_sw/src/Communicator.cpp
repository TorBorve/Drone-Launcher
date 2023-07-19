#include "Communicator.h"

#include "LaunchSystem.h"
#include "Log.h"
#include "Pins.h"

#define COM_UPDATE_INTERVAL 100
#define COM_STATUS_UPDATE_INTERVAL 1000

Communicator communicator{};

Communicator::Communicator() : nh{},
                               _statusPub{"drone_launcher/status", &_statusMsg},
                               _fireSub{"drone_launcher/fire", &fireCallback},
                               _loadSub{"drone_launcher/load", &loadCallback},
                               _unloadSub{"drone_launcher/unload", &unloadCallback},
                               _init{false},
                               _aliveLed{PIN_LED_ALIVE, BaseLed::Mode::BLINK},
                               _prevUpdate{0},
                               _prevStatusUpdate{0} {}

void Communicator::init() {
    if (!_init) {
        _statusMsg.launchUnits = _luStatusMsgs;
        _statusMsg.launchUnits_length = LS_NUM_UNITS;
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
    if (!_init || now - _prevUpdate < COM_UPDATE_INTERVAL) {
        return;
    }
    _prevUpdate = now;

    if (now - _prevStatusUpdate > COM_STATUS_UPDATE_INTERVAL) {
        _prevStatusUpdate = now;
        updateStatus(now);
        _statusPub.publish(&_statusMsg);
    }
    nh.spinOnce();  // Not thread safe since nh is used in logging.
    _aliveLed.update(now);
}

void Communicator::updateStatus(uint32_t now) {
    _statusMsg.header.stamp = nh.now();
    _statusMsg.header.frame_id = "drone_launcher1";
    _statusMsg.armed = launchSystem.isArmed();
    _statusMsg.batteryVoltage = -1;  // TODO
    // _statusMsg.gps TODO
    // _statusMsg.orientation TODO
    for (uint8_t i = 0; i < LS_NUM_UNITS; i++) {
        _luStatusMsgs[i].state = toMsg(launchSystem.getLaunchUnitState(i));  // TODO: use function instead of cast
        _luStatusMsgs[i].loadedDroneId = launchSystem.getLoadedDroneId(i);
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

int8_t Communicator::toMsg(LaunchUnit::State state) {
    switch (state) {
        case LaunchUnit::State::FIRED:
            return drone_launcher_msgs_pkg::LaunchUnitStatus::STATE_FIRED;
        case LaunchUnit::State::LOADING:
            return drone_launcher_msgs_pkg::LaunchUnitStatus::STATE_LOADING;
        case LaunchUnit::State::LOADED:
            return drone_launcher_msgs_pkg::LaunchUnitStatus::STATE_LOADED;
        case LaunchUnit::State::FIRING:
            return drone_launcher_msgs_pkg::LaunchUnitStatus::STATE_FIRING;
        case LaunchUnit::State::UNLOADING:
            return drone_launcher_msgs_pkg::LaunchUnitStatus::STATE_UNLOADING;
        case LaunchUnit::State::ERROR:
            return drone_launcher_msgs_pkg::LaunchUnitStatus::STATE_ERROR;
        default:
            LOG_ERROR("Unknown launch unit state in toMsg funciton");
            return -1;
    }
}
