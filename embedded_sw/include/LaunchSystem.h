#pragma once

#include "LaunchUnit.h"
#include "Pins.h"

const uint8_t _launchUnitPins[][5] = {
    {PIN_F1R1_TRIGGER_SERVO, PIN_F1R1_SAFETY_SERVO, PIN_F1R1_REAR_SWITCH, PIN_F1R1_SAFETY_SWITCH, PIN_F1R1_FRONT_SWITCH},
    {PIN_F1R2_TRIGGER_SERVO, PIN_F1R2_SAFETY_SERVO, PIN_F1R2_REAR_SWITCH, PIN_F1R2_SAFETY_SWITCH, PIN_F1R2_FRONT_SWITCH},
    // {PIN_F2R1_TRIGGER_SERVO, PIN_F2R1_SAFETY_SERVO, PIN_F2R1_SAFETYdd_SWITCH, PIN_F2R1_REARdd_SWITCH, PIN_F2R1_FRONT_SWITCH},
    // {PIN_F2R2_TRIGGER_SERVO, PIN_F2R2_SAFETY_SERVO, PIN_F2R2_SAFETddY_SWITCH, PIN_F2R2_REARdd_SWITCH, PIN_F2R2_FRONT_SWITCH},
//     {PIN_F3R1_TRIGGER_SERVO, PIN_F3R1_SAFETY_SERVO, PIN_F3R1_SAFETYdd_SWITCH, PIN_F3R1_REAR_SWITCH, PIN_F3R1_FRONT_SWITCH},
//     {PIN_F3R2_TRIGGER_SERVO, PIN_F3R2_SAFETY_SERVO, PIN_F3R2_SAFETY_dddSWITCH, PIN_F3R2_REAR_ddSWITCH, PIN_F3R2_FRONT_SWITCH},
};

#define LS_NUM_UNITS (sizeof(_launchUnitPins) / sizeof(_launchUnitPins[0]))

using DroneId = uint8_t;

class LaunchSystem {
   public:
    LaunchSystem();
    void init();
    void update(uint32_t now);
    // State getState() const;
    void fire(uint8_t launchUnitId);
    void load(uint8_t launchUnitId, DroneId droneId);
    void unload(uint8_t launchUnitId);
    bool isArmed() const { return _isArmed;}
    LaunchUnit::State getLaunchUnitState(uint8_t launchUnitId) const { return _launchUnits[launchUnitId].getState(); }
    DroneId getLoadedDroneId(uint8_t launchUnitId) const { return _loadedDroneIds[launchUnitId]; }

   private:
   #pragma message "TODO: Add vector containing available drones"
    LaunchUnit _launchUnits[LS_NUM_UNITS];
    DroneId _loadedDroneIds[LS_NUM_UNITS];
    CRGB _statusLeds[LS_NUM_UNITS];
    Switch _armSwitch;
    bool _isArmed;
    uint32_t _prevUpdate;
};

extern LaunchSystem launchSystem;