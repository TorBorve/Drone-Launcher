#pragma once

#include <Arduino.h>
#include <FastLED.h>
#define BM_NUM_LED 6

class BatteryMonitor {
    private:
        void readBat();
        uint8_t _batMeasPin;
        float _voltage;
        CRGB _ledArray[BM_NUM_LED];
        uint32_t _prevUpdate;

    public:
        static float voltageToPercent(float voltage);
        void update(uint32_t now);
        void init();
        float getVoltage() {return _voltage;}
        float getPercent() {return voltageToPercent(_voltage);}
        BatteryMonitor();
};

extern BatteryMonitor batteryMonitor;