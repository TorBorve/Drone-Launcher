#include "GuidanceSystem.h"

// GuidanceSystem::GuidanceSystem() : _gps(), _imu() {
//     }

void GuidanceSystem::init() {
    _imu.init();
    _gps.init();
}

void GuidanceSystem::update(uint32_t now) {
    _imu.update(now);
    _gps.update(now);
}

bool GuidanceSystem::status(uint32_t now) {
    return _imu.status(now) && _gps.status(now);
}