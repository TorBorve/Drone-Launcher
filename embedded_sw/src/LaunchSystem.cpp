#include "LaunchSystem.h"

#define LS_ARM_SWITCH_PIN 254

const uint8_t launchUnitPins[LS_NUM_UNITS][5]{
    {21, 22, 3, 4, 5}};

LaunchSystem::LaunchSystem() : _launchUnits{
                                   LaunchUnit{launchUnitPins[0][0],
                                              launchUnitPins[0][1], 
                                              launchUnitPins[0][2], 
                                              launchUnitPins[0][3], 
                                              launchUnitPins[0][4], 
                                              _statusLeds[0]}},
                                _armSwitch{LS_ARM_SWITCH_PIN}, 
                                _isArmed{false} {
}

void LaunchSystem::init() {
    for (uint8_t i = 0; i < LS_NUM_UNITS; i++) {
        _loadedDroneIds[i] = 0;
        _statusLeds[i] = CRGB::Black;
        _launchUnits[i].init();
    }
    _armSwitch.init();
}

void LaunchSystem::update(uint32_t now) {
    _armSwitch.poll(now);
    if (_armSwitch.hasEvent()) {
        _isArmed = _armSwitch.getState();
        for (uint8_t i = 0; i < LS_NUM_UNITS; i++) {
            _launchUnits[i].setArmed(_isArmed);
        }
    }
    for (uint8_t i = 0; i < LS_NUM_UNITS; i++) {
        _launchUnits[i].update(now);
    }
}

void LaunchSystem::fire(uint8_t launchUnitId) {
    if (_isArmed) {
        _launchUnits[launchUnitId].fire();
        _loadedDroneIds[launchUnitId] = 0;
    }
}

void LaunchSystem::load(uint8_t launchUnitId, DroneId droneId) {
    if (!_isArmed) {
        _launchUnits[launchUnitId].load();
        #pragma warning "TODO: make sure load was successful...."
        _loadedDroneIds[launchUnitId] = droneId; // TODO: make sure load was successful....
    }
}

void LaunchSystem::unload(uint8_t launchUnitId) {
    if (!_isArmed) {
        _launchUnits[launchUnitId].unload();
        _loadedDroneIds[launchUnitId] = 0;
    }
}

