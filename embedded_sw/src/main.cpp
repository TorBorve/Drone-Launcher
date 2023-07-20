#include <Arduino.h>

#include "Pins.h"
#include "LaunchSystem.h"
#include "Communicator.h"
#include "Menu.h"
#include "Log.h"

void setup() {
    communicator.init();
    pins::setup();
    launchSystem.init();
    menu.init();
    LOG_INFO("Startup complete");
}

void loop() {
    uint32_t now = millis();
    launchSystem.update(now);
    communicator.update(now);
    menu.update(now);
}