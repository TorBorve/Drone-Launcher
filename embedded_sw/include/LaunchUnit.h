#pragma once

#include <Servo.h>
#include <TeensyThreads.h>

#include "Buttons.h"
#include "Leds.h"

namespace DroneLauncher {

// class for controlling a launch unit (one platform/chamber)
class LaunchUnit {
   public:
    enum class State : uint8_t { FIRED,
                                 LOADING,
                                 LOADED,
                                 FIRING,
                                 UNLOADING,
                                 ERROR };
    LaunchUnit(uint8_t triggerServoPin, uint8_t safetyServoPin, uint8_t rearSwitchPin, uint8_t safetySwitchPin, CRGB& statusLed, bool mirrored);
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
    static float triggerAngle(bool servoOn, bool mirrored);
    State _state;
    Servo _safetyServo;
    Servo _triggerServo;
    const uint8_t _safetyServoPin;
    const uint8_t _triggerServoPin;
    Switch _rearSwitch;
    Switch _safetySwitch;
    RGBLed _statusLed;
    bool _isArmed;
    const bool _mirrored;  // if the trigger servo is mirrored (i.e. the servo is mounted on the other side of the platform)
    mutable Threads::Mutex _mutex;
};

}  // namespace DroneLauncher