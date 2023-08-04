#include <Arduino.h>

#include "LaunchSystem.h"
#include "BatteryMonitor.h"
#include "Communicator.h"
#include "Navigator.h"
#include "Log.h"
#include "Menu.h"
#include "Pins.h"


void setup() {
    pins::setup();
    launchSystem.init();
    batteryMonitor.init();
    communicator.init();
    menu.init();
    navigator.init();
    LOG_INFO("Setup complete");
}

void loop() {
    uint32_t now = millis();
    launchSystem.update(now);
    batteryMonitor.update(now);
    communicator.update(now);
    menu.update(now);
    navigator.update(now);
}