#include "Pins.h"

#include "Log.h"

namespace pins {

Adafruit_MCP23X17 mcp;

void setup() {
    if (!mcp.begin_I2C()) {
        while (1) {
            LOG_ERROR("MCP23X17 not found");
            delay(1000);
        }
    }
    LOG_INFO("Pins initialized");
}
}  // namespace pins