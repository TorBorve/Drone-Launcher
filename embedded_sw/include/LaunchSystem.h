#pragma once

#include "LaunchUnit.h"
#include "Pins.h"

namespace DroneLauncher {

// Pins for all different launch units
const uint8_t _launchUnitPins[][5] = {
    {PIN_F1R1_TRIGGER_SERVO, PIN_F1R1_SAFETY_SERVO, PIN_F1R1_REAR_SWITCH, PIN_F1R1_SAFETY_SWITCH},
    {PIN_F1R2_TRIGGER_SERVO, PIN_F1R2_SAFETY_SERVO, PIN_F1R2_REAR_SWITCH, PIN_F1R2_SAFETY_SWITCH},
};

#define LS_NUM_UNITS (sizeof(_launchUnitPins) / sizeof(_launchUnitPins[0]))

// Class for sending and reciving commands from all the launch units
class LaunchSystem {
   public:
    using DroneId = uint8_t;
    LaunchSystem();
    void init();
    void update(uint32_t now);
    void fire(uint8_t launchUnitId);
    void load(uint8_t launchUnitId, DroneId droneId);
    void unload(uint8_t launchUnitId);
    void manualControl(uint8_t launchUnitId, bool safetyOn, bool triggerOn);
    bool isArmed() const { return _isArmed; }
    LaunchUnit::State getLaunchUnitState(uint8_t launchUnitId) const;
    DroneId getLoadedDroneId(uint8_t launchUnitId) const;

   private:
    LaunchUnit _launchUnits[LS_NUM_UNITS];
    DroneId _loadedDroneIds[LS_NUM_UNITS];
    CRGB _statusLeds[LS_NUM_UNITS];
    Switch _armSwitch;
    bool _isArmed;
    uint32_t _prevUpdate;
};

extern LaunchSystem launchSystem;

}  // namespace DroneLauncher