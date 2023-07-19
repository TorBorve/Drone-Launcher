#include "BatteryMonitor.h"


#define PIN_BAT_LEDS 25
#define PIN_BAT_MEAS 23
#define MIN_BAT_VOLTAGE (3.6 * 6)
#define MAX_BAT_VOLTAGE (4.2 * 6)
#define BRIGHTNESS  64
#define LED_TYPE    WS2811
#define COLOR_ORDER GRB
#define UPDATE_INTERVAL 1000


BatteryMonitor batteryMonitor;

BatteryMonitor::BatteryMonitor() {
    _pin_value = -1;
    _voltage = -1;
    _percentage = -1;
    _prevUpdate = 0;
    for (int i = 0; i < BM_NUM_LED; i++) {
        _led_array[i] = CRGB::Black;
    }
}

void BatteryMonitor::init() {
    for (int i = 0; i < BM_NUM_LED; i++) {
        _led_array[i] = CRGB::White;
    }
    FastLED.addLeds<LED_TYPE, PIN_BAT_LEDS, COLOR_ORDER>(_led_array, BM_NUM_LED).setCorrection( TypicalLEDStrip );
    FastLED.setBrightness(  BRIGHTNESS );
    FastLED.show();
    readBat();
}

void BatteryMonitor::update(uint32_t now) {
    if (now - _prevUpdate < UPDATE_INTERVAL) {
        return;
    }
    _prevUpdate = now;
    // Update reading
    readBat();

    // Update LED value array
    for (int i = 0; i < BM_NUM_LED; i++) {
        if ((1.0 / BM_NUM_LED) * i < _percentage) {
            _led_array[i] = CRGB::Green;
        } else {
            _led_array[i] = CRGB::Red;
        }
    }
    
    // Update LEDS
    FastLED.show();
}

void BatteryMonitor::readBat() {
    _pin_value = analogRead(PIN_BAT_MEAS);
    _voltage = _pin_value * (3.3 / 1024.0) * (940.0 / 120.0);
    _percentage = (_voltage - MIN_BAT_VOLTAGE) / (MAX_BAT_VOLTAGE - MIN_BAT_VOLTAGE);
}