#include "LaunchUnit.h"

LaunchUnit::LaunchUnit(uint8_t servoPin) : _servoPin{servoPin} {
    _triggerServo.attach(servoPin);
    _triggerServo.write(LU_SERVO_LOADED);
}

bool LaunchUnit::isLoaded() const {
    return true;
}

void LaunchUnit::launch() {
    _triggerServo.write(LU_SERVO_TRIGGERED);
}