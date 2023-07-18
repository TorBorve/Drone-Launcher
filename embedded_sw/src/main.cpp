#include <Arduino.h>

#include "Pins.h"
#include "LaunchSystem.h"
#include "Communicator.h"

void setup() {
    delay(3000);
    // Serial.println("Setup started");
    pins::setup();
    launchSystem.init();
    communicator.init();
    Serial.println("Setup complete");
}

void loop() {
    uint32_t now = millis();
    launchSystem.update(now);
    communicator.update(now);
}