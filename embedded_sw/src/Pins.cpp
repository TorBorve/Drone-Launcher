#include "Pins.h"

namespace pins {

Adafruit_MCP23X17 mcp;

void setup() {
    if (!mcp.begin_I2C()) {
        Serial.println("Failed to find MCP23X17");
        while (1)
            ;
    }
}
}  // namespace pins