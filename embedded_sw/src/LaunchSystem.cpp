#include "LaunchSystem.h"

#include <FastLED.h>

#include "Log.h"

#define LS_UPDATE_INTERVAL 50

LaunchSystem launchSystem;

LaunchSystem::LaunchSystem() : _launchUnits{
                                   LaunchUnit{_launchUnitPins[0][0],
                                              _launchUnitPins[0][1],
                                              _launchUnitPins[0][2],
                                              _launchUnitPins[0][3],
                                              _launchUnitPins[0][4],
                                              _statusLeds[0],
                                              false},
                                   LaunchUnit{_launchUnitPins[1][0],
                                              _launchUnitPins[1][1],
                                              _launchUnitPins[1][2],
                                              _launchUnitPins[1][3],
                                              _launchUnitPins[1][4],
                                              _statusLeds[1],
                                              true},
                                //    LaunchUnit{_launchUnitPins[2][0],
                                //               _launchUnitPins[2][1],
                                //               _launchUnitPins[2][2],
                                //               _launchUnitPins[2][3],
                                //               _launchUnitPins[2][4],
                                //               _statusLeds[2]},
                                //    LaunchUnit{_launchUnitPins[3][0],
                                //               _launchUnitPins[3][1],
                                //               _launchUnitPins[3][2],
                                //               _launchUnitPins[3][3],
                                //               _launchUnitPins[3][4],
                                //               _statusLeds[3]},
                                //    LaunchUnit{_launchUnitPins[4][0],
                                //               _launchUnitPins[4][1],
                                //               _launchUnitPins[4][2],
                                //               _launchUnitPins[4][3],
                                //               _launchUnitPins[4][4],
                                //               _statusLeds[4]},
                                //    LaunchUnit{_launchUnitPins[5][0],
                                //               _launchUnitPins[5][1],
                                //               _launchUnitPins[5][2],
                                //               _launchUnitPins[5][3],
                                //               _launchUnitPins[5][4],
                                //               _statusLeds[5]},

                               },
                               _armSwitch{PIN_ARM_SWITCH},
                               _isArmed{false},
                               _prevUpdate{0} {
}

void LaunchSystem::init() {
    for (uint8_t i = 0; i < LS_NUM_UNITS; i++) {
        _loadedDroneIds[i] = 0;
        _statusLeds[i] = CRGB::Black;
        _launchUnits[i].init();
    }
    _armSwitch.init();
    FastLED.addLeds<WS2812, PIN_DRONE_LEDS, GRB>(_statusLeds, LS_NUM_UNITS);
    FastLED.show();
    LOG_INFO("Launch system initialized");
}

void LaunchSystem::update(uint32_t now) {
    _armSwitch.poll(now);
    for (uint8_t i = 0; i < LS_NUM_UNITS; i++) {
        _launchUnits[i].update(now);
    }
    if (now - _prevUpdate > LS_UPDATE_INTERVAL) {
        _prevUpdate = now;
        if (_armSwitch.hasEvent()) {
            _armSwitch.clearEvent();
            _isArmed = _armSwitch.getState();
            for (uint8_t i = 0; i < LS_NUM_UNITS; i++) {
                _launchUnits[i].setArmed(_isArmed);
            }
        }
        FastLED.show();
    }
}

void LaunchSystem::fire(uint8_t launchUnitId) {
    if (_isArmed) {
        _launchUnits[launchUnitId].fire();
        _loadedDroneIds[launchUnitId] = 0;
    } else {
        LOG_WARN("Attempted to fire launch unit %d while launch system is not armed", launchUnitId);
    }
}

void LaunchSystem::load(uint8_t launchUnitId, DroneId droneId) {
    if (!_isArmed && launchUnitId >= 0 && launchUnitId < LS_NUM_UNITS) {
        _launchUnits[launchUnitId].load();
#pragma warning "TODO: make sure load was successful...."
        _loadedDroneIds[launchUnitId] = droneId;  // TODO: make sure load was successful....
    } else {
        LOG_WARN("Attempted to load launch unit %d while launch system is armed", launchUnitId);
    }
}

void LaunchSystem::unload(uint8_t launchUnitId) {
    if (!_isArmed && launchUnitId >= 0 && launchUnitId < LS_NUM_UNITS) {
        _launchUnits[launchUnitId].unload();
        _loadedDroneIds[launchUnitId] = 0;
    }
}
