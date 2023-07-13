#pragma once

#include <Servo.h>
#include <FastLED.h>

#include <TeensyThreads.h>

#include "Buttons.h"

class LaunchUnit {
public:
    enum class State : uint8_t {FIRED, LOADING, LOADED, FIRING, ERROR};
    LaunchUnit(uint8_t triggerServoPin, uint8_t safetyServoPin, uint8_t rearSwitchPin, uint8_t safetySwitchPin, uint8_t frontSwitchPin, CRGB& statusLed);
    void update(uint32_t now);
    // bool isLoaded() const;
    // void launch();
    // void load();
    void setArmed(bool isArmed);
private:
    void updateLed();
    static void loadThread(void* arg);
    static void fireThread(void* arg);
    State _state;
    Servo _safetyServo;
    Servo _triggerServo;
    Switch _rearSwitch;
    Switch _safetySwitch;
    Switch _frontSwitch;
    CRGB& _statusLed;
    uint32_t _prevUpdate;
    bool _isArmed;
    Threads::Mutex _mutex;
};