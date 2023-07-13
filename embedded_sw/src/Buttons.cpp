#include <Arduino.h>
#include <Buttons.h>

#define ON_THRESHOLD 0.8      // Value of _state before we say the button is pushed
#define FILTER_TRUST_NEW 0.15  // How much do we trust the new measurement
#define FILTER_BIAS 0.3       // How much more/less than 0.5 must the state be before we change the bool state
#define BUTTON_INTERVAL 5   // update interval in ms. Ensure consisten freq in updates.

// constructor
Switch::Switch(uint8_t pin) : _pin{pin}, _state{0.0}, _boolState{false}, _hasChanged{false}, _prevUpdate{0} {
    pinMode(_pin, INPUT_PULLUP);
}

// refresh the button state
void Switch::poll(uint32_t now) {
    if (now - _prevUpdate < BUTTON_INTERVAL) {
        return;
    }
    _prevUpdate = now;

    // pullup resistors cause the
    //  button to be HIGH when open
    //  so we invert the hardware state
    _state = (1 - FILTER_TRUST_NEW) * _state + FILTER_TRUST_NEW * !digitalRead(_pin);

    bool newBoolState = calcBoolState(_state, _boolState);
    if (newBoolState != _boolState) {
        _hasChanged = true;
        _boolState = newBoolState;
    }
}

bool Switch::calcBoolState(float state, bool boolState) {
    float bias = -FILTER_BIAS * (2 * (int)boolState - 1);  // last part is to set correct sign of bias.
    return state > (0.5 + FILTER_BIAS) ? true : false;
}

// reset the button
void Switch::reset(void) {
    *this = Switch{_pin};
}