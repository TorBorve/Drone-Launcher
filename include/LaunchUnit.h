#pragma once

#include <Servo.h>

#define LU_SERVO_LOADED 45
#define LU_SERVO_TRIGGERED 90

class LaunchUnit {
public:
    LaunchUnit(uint8_t servoPin);
    bool isLoaded() const;
    void launch();
    // void load();

private:
    Servo _triggerServo;
    uint8_t _servoPin;
};