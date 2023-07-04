#include <Arduino.h>

#include "Button.h"
#include "LaunchUnit.h"
#include "cmath"

#define SAFETY_SERVO_PIN 2
#define SAFETY_BUTTON_PIN 3
#define TRIGGER_BUTTON_PIN 20
#define TRIGGER_SERVO_PIN 21
#define SAFETY_SERVO_MIN_ANGLE 180
#define SAFETY_SERVO_MAX_ANGLE 130

Button triggerBtn{TRIGGER_BUTTON_PIN};
Button safetyBtn{SAFETY_BUTTON_PIN};
Servo triggerServo;
// LaunchUnit launchUnit{TRIGGER_SERVO_PIN};
Servo safetyServo;


void setup() {
    delay(1000);
    Serial.println("Setup");
    triggerServo.attach(TRIGGER_SERVO_PIN);
    triggerServo.write(LU_SERVO_LOADED);
    safetyServo.attach(SAFETY_SERVO_PIN);
    safetyServo.write(SAFETY_SERVO_MIN_ANGLE);
}

void loop() {
    uint32_t now = millis();
    safetyBtn.poll(now);
    triggerBtn.poll(now);
    if (triggerBtn.wasPressed()) {
        Serial.println("Trigger Btn pressed");
        if (triggerServo.read() == LU_SERVO_LOADED) {
            triggerServo.write(LU_SERVO_TRIGGERED);
        } else {
            triggerServo.write(LU_SERVO_LOADED);
        }
        // launchUnit.launch();
        triggerBtn.reset();
        delay(3000);
    }
    if (safetyBtn.wasPressed()) {
        Serial.println("Safety btn pressed");

        if (std::fabs(safetyServo.read() - SAFETY_SERVO_MIN_ANGLE) < 5) {
            safetyServo.write(SAFETY_SERVO_MAX_ANGLE);
        } else {
            safetyServo.write(SAFETY_SERVO_MIN_ANGLE);
        }
        safetyBtn.reset();
        delay(3000);
    }
}