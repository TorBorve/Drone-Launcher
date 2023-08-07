#include <Arduino.h>

#include "BatteryMonitor.h"
#include "Communicator.h"
#include "LaunchSystem.h"
#include "Log.h"
#include "Menu.h"
#include "Navigator.h"
#include "Pins.h"

void setup() {
    DroneLauncher::pins::setup();
    DroneLauncher::launchSystem.init();
    DroneLauncher::batteryMonitor.init();
    DroneLauncher::communicator.init();
    DroneLauncher::menu.init();
    DroneLauncher::navigator.init();
    LOG_INFO("Setup complete");
}

void loop() {
    uint32_t now = millis();
    DroneLauncher::launchSystem.update(now);
    DroneLauncher::batteryMonitor.update(now);
    DroneLauncher::communicator.update(now);
    DroneLauncher::menu.update(now);
    DroneLauncher::navigator.update(now);
}