#pragma once

#include <stdint.h>

namespace DroneLauncher {

// Button classs, encapsulate the functionality of a button
class Switch {
   protected:
    static bool calcBoolState(float state, bool boolState);

    uint8_t _pin;  // hardware pin number
    float _state;  // current pin state
    bool _boolState; // state as pushed/not pushed
    bool _hasChanged;  // button latch
    uint32_t _prevUpdate; // time in ms of previous update

   public:
    Switch(uint8_t pin);
    void init();
    void poll(uint32_t now);                         // call periodically to refresh the button state
    virtual bool hasEvent() const { return _hasChanged; }  // return the latch state
    void clearEvent() {_hasChanged = false;}
    bool getState() const { return _boolState; }
    void reset(void);
};

class Button : public Switch {
    using Switch::Switch; // use constructors from switch
  public:
    virtual bool hasEvent() const override { return _hasChanged && _boolState;} // only alert us new button is pushed
};

} // namespace DroneLauncher