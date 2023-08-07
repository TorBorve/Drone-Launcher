#pragma once

#include <Arduino.h>
#include <FastLED.h>
#define BM_NUM_LED 5

namespace DroneLauncher {

// Class for monitoring the battery voltage
class BatteryMonitor {
   public:
    BatteryMonitor();
    void update(uint32_t now);
    void init();
    float getVoltage() { return _voltage; }
    float getPercent() { return voltageToPercent(_voltage); }
    static float voltageToPercent(float voltage);

   private:
    float readBat();
    uint8_t _batMeasPin;
    float _voltage;
    CRGB _ledArray[BM_NUM_LED];
    uint32_t _prevUpdate;
};

extern BatteryMonitor batteryMonitor;

}  // namespace DroneLauncher