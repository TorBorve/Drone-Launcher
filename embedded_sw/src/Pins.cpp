#include "Pins.h"

#include "Log.h"

namespace DroneLauncher {

namespace pins {

Adafruit_MCP23X17 mcp;

void setup() {
    pinMode(PIN_RESET_MCP, OUTPUT);
    digitalWrite(PIN_RESET_MCP, LOW);
    delay(100);
    digitalWrite(PIN_RESET_MCP, HIGH);
    delay(100);
    pinMode(PIN_RESET_MCP, INPUT);
    delay(100);
    if (!mcp.begin_I2C(PINS_MCP_ADDR)) {
        while (1) {
            LOG_ERROR("MCP23X17 not found");
            delay(1000);
        }
    }
    LOG_INFO("Pins initialized");
}
}  // namespace pins

}  // namespace DroneLauncher