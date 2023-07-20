#pragma once

#include <Servo.h>

#include <TeensyThreads.h>

#include "Buttons.h"
#include "Leds.h"

class LaunchUnit {
public:
    enum class State : uint8_t {FIRED, LOADING, LOADED, FIRING, UNLOADING, ERROR};
    LaunchUnit(uint8_t triggerServoPin, uint8_t safetyServoPin, uint8_t rearSwitchPin, uint8_t safetySwitchPin, uint8_t frontSwitchPin, CRGB& statusLed);
    void init();
    void update(uint32_t now);
    void fire();
    void load();
    void unload();
    State getState() const;
    void setArmed(bool isArmed);
private:
    static void loadThread(void* arg);
    static void fireThread(void* arg);
    static void unloadThread(void* arg);
    void updateLed();
    State _state;
    Servo _safetyServo;
    Servo _triggerServo;
    Switch _rearSwitch;
    Switch _safetySwitch;
    Switch _frontSwitch;
    RGBLed _statusLed;
    bool _isArmed;
    mutable Threads::Mutex _mutex;
};