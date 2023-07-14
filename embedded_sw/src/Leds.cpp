#include "Leds.h"

#define BLINK_INTERVAL 500

////////////////////////////////////////
// BaseLed
////////////////////////////////////////

BaseLed::BaseLed(Mode mode) : _mode{mode}, _isOn{mode == Mode::ON}, _prevBlink(0) {
}

void BaseLed::update(uint32_t now) {
    if (_mode == Mode::BLINK && now - _prevBlink > BLINK_INTERVAL) {
        _prevBlink = now;
        _isOn = !_isOn;
        writeToLed();
    }
}

void BaseLed::setMode(Mode mode) {
    _mode = mode;
    if (_mode != Mode::BLINK) {
        _isOn = _mode == Mode::ON;
    }
    writeToLed();
}

////////////////////////////////////////
// Led
////////////////////////////////////////

Led::Led(uint8_t pin, Mode mode) : BaseLed(mode), _pin{pin} {
    pinMode(_pin, OUTPUT);
    writeToLed();
}

void Led::writeToLed() {
    digitalWrite(_pin, _isOn ? HIGH : LOW);
}

////////////////////////////////////////
// RGBLed
////////////////////////////////////////

RGBLed::RGBLed(CRGB& led, Mode mode) : BaseLed(mode), _actualColor{led}, _color{led} {
    writeToLed();
}

void RGBLed::writeToLed() {
    if (_isOn) {
        _actualColor = _color;
    } else {
        _actualColor = CRGB::Black;
    }
}
