#include "LaunchSystem.h"

#include <FastLED.h>

#include "Log.h"

#define LS_UPDATE_INTERVAL 50

namespace DroneLauncher {

LaunchSystem launchSystem;

LaunchSystem::LaunchSystem() : _launchUnits{
                                   LaunchUnit{_launchUnitPins[0][0],
                                              _launchUnitPins[0][1],
                                              _launchUnitPins[0][2],
                                              _launchUnitPins[0][3],
                                              _statusLeds[0],
                                              false},
                                   LaunchUnit{_launchUnitPins[1][0],
                                              _launchUnitPins[1][1],
                                              _launchUnitPins[1][2],
                                              _launchUnitPins[1][3],
                                              _statusLeds[1],
                                              true},
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
    FastLED.addLeds<WS2812, PIN_DRONE_LEDS, GRB>(_statusLeds, LS_NUM_UNITS).setCorrection(TypicalLEDStrip);
    FastLED.show();
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
    if (_isArmed && launchUnitId >= 0 && launchUnitId < LS_NUM_UNITS) {
        _launchUnits[launchUnitId].fire();
        _loadedDroneIds[launchUnitId] = 0;
    } else {
        LOG_WARN("Attempted to fire launch unit %d while launch system is not armed OR launch unit ID out of range", launchUnitId);
    }
}

void LaunchSystem::load(uint8_t launchUnitId, DroneId droneId) {
    if (!_isArmed && launchUnitId >= 0 && launchUnitId < LS_NUM_UNITS) {
        _launchUnits[launchUnitId].load();
        _loadedDroneIds[launchUnitId] = droneId;  // TODO: make sure load was successful....
    } else {
        LOG_WARN("Attempted to load launch unit %d while launch system is armed OR launch unit ID out of range", launchUnitId);
    }
}

void LaunchSystem::unload(uint8_t launchUnitId) {
    if (!_isArmed && launchUnitId >= 0 && launchUnitId < LS_NUM_UNITS) {
        _launchUnits[launchUnitId].unload();
        _loadedDroneIds[launchUnitId] = 0;
    } else {
        LOG_WARN("Attempted to unload launch unit %d while launch system is armed OR launch unit ID out of range", launchUnitId);
    }
}

void LaunchSystem::manualControl(uint8_t launchUnitId, bool safetyOn, bool triggerOn) {
    if (launchUnitId >= 0 && launchUnitId < LS_NUM_UNITS) {
        _launchUnits[launchUnitId].manualControl(safetyOn, triggerOn);
    } else {
        LOG_WARN("Attempted to manually control launch unit %d, however launch unit ID out of range", launchUnitId);
    }
}

LaunchUnit::State LaunchSystem::getLaunchUnitState(uint8_t launchUnitId) const {
    if (launchUnitId >= 0 && launchUnitId < LS_NUM_UNITS) {
        return _launchUnits[launchUnitId].getState();
    } else {
        return LaunchUnit::State::ERROR;
    }
}

LaunchSystem::DroneId LaunchSystem::getLoadedDroneId(uint8_t launchUnitId) const {
    if (launchUnitId >= 0 && launchUnitId < LS_NUM_UNITS) {
        return _loadedDroneIds[launchUnitId];
    } else {
        return 0;
    }
}

}  // namespace DroneLauncher