#include "BatteryMonitor.h"

#include "Pins.h"

namespace DroneLauncher {

#define BAT_SCALE 7.835306
#define MIN_BAT_VOLTAGE (3.6 * 6)
#define MAX_BAT_VOLTAGE (4.2 * 6)
#define BRIGHTNESS  64
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define UPDATE_INTERVAL 1000


BatteryMonitor batteryMonitor;

BatteryMonitor::BatteryMonitor() : _batMeasPin{PIN_BAT_MEAS}, _voltage{0}, _prevUpdate{0} {
    for (int i = 0; i < BM_NUM_LED; i++) {
        _ledArray[i] = CRGB::Black;
    }
}

void BatteryMonitor::init() {
    FastLED.addLeds<LED_TYPE, PIN_BAT_LEDS, COLOR_ORDER>(_ledArray, BM_NUM_LED).setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(BRIGHTNESS);
    FastLED.show();

    pinModeExt(_batMeasPin, INPUT);
    _voltage = readBat();
}

void BatteryMonitor::update(uint32_t now) {
    if (now - _prevUpdate < UPDATE_INTERVAL) {
        return;
    }
    _prevUpdate = now;
    // Update reading
    _voltage = readBat();

    // Update LED value array
    float percentage = voltageToPercent(_voltage);
    for (int i = 0; i < BM_NUM_LED; i++) {
        if ((1.0 / BM_NUM_LED) * i < percentage) {
            _ledArray[i] = CRGB::Green;
        } else {
            _ledArray[i] = CRGB::Red;
        }
    }
    
    // Update LEDS
    FastLED.show();
}

float BatteryMonitor::readBat() {
    return analogRead(_batMeasPin) * (3.3 / 1024.0) * BAT_SCALE;
}

float BatteryMonitor::voltageToPercent(float voltage) {
    return min(1, max(0,(voltage - MIN_BAT_VOLTAGE) / (MAX_BAT_VOLTAGE - MIN_BAT_VOLTAGE)));
}

}  // namespace DroneLauncher