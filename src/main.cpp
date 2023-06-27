#include <Arduino.h>

#include "Button.h"
#include "LaunchUnit.h"

#define BUTTON_PIN 20
#define SERVO_PIN 21

Button btn{BUTTON_PIN};
LaunchUnit launchUnit{SERVO_PIN};

void setup() {
    delay(1000);
    Serial.println("Setup");
}

void loop() {
    btn.poll(millis());
    if (btn.wasPressed()) {
        Serial.println("Btn pressed");
        launchUnit.launch();
    }
}