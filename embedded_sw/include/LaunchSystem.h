#pragma once

#include "LaunchUnit.h"

#define LS_NUM_UNITS 1

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

   private:
   #pragma message "TODO: Add vector containing available drones"
    LaunchUnit _launchUnits[LS_NUM_UNITS];
    DroneId _loadedDroneIds[LS_NUM_UNITS];
    CRGB _statusLeds[LS_NUM_UNITS];
    Switch _armSwitch;
    bool _isArmed;
};