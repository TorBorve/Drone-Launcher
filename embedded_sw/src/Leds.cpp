#include "Leds.h"

#include "Pins.h"

namespace DroneLauncher {

#define BLINK_INTERVAL 500

////////////////////////////////////////
// BaseLed
////////////////////////////////////////

BaseLed::BaseLed(Mode mode) : _mode{mode}, _isOn{mode == Mode::ON} {
}

void BaseLed::update(uint32_t now) {
    if (_mode == Mode::ON || _mode == Mode::OFF) {
        return;
    }
    bool prevIsOn = _isOn;
    if (now % (2 * BLINK_INTERVAL) > BLINK_INTERVAL) {
        _isOn = true;
    } else {
        _isOn = false;
    }

    if (prevIsOn != _isOn) {
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
}

void Led::init() {
    pinModeExt(_pin, OUTPUT);
    writeToLed();
}

void Led::writeToLed() {
    digitalWriteExt(_pin, _isOn ? HIGH : LOW);
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

}  // namespace DroneLauncher
