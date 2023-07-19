#pragma once

#include <Arduino.h>
#include <FastLED.h>
#define BM_NUM_LED 5

class BatteryMonitor {
    private:
        int _pin_value;
        float _battery_percentage;
        float _voltage;
        float _percentage;
        void readBat();
        CRGB _led_array[BM_NUM_LED];
        uint32_t _prevUpdate;

    public:
        void update(uint32_t now);
        void init();
        float getVoltage() {return _voltage;}
        float getPercent() {return _percentage;}
        BatteryMonitor();
};

extern BatteryMonitor batteryMonitor;