#pragma once

#include <Servo.h>

#define LU_SERVO_LOADED 0
#define LU_SERVO_TRIGGERED 180

class LaunchUnit {
public:
    enum class State : uint8_t {FIRED, LOADED, ERROR};
    LaunchUnit(uint8_t servoPin);
    bool isLoaded() const;
    void launch();
    void load();
    void update();
private:
    State _state;
    Servo _triggerServo;
    uint8_t _servoPin;
    uint8_t _loadedBtnPin;
    uint8_t _firedBtnPin;
};