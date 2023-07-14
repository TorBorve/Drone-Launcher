#pragma once

#include <Adafruit_MCP23X17.h>

namespace pins {

Adafruit_MCP23X17 mcp;

#define PINS_MCP_OFFSET 64

#define pinModeExt(pin, mode) (pin > PINS_MCP_OFFSET) ? pins::mcp.pinMode(pin - PINS_MCP_OFFSET, mode) : pinMode(pin, mode)

#define digitalWriteExt(pin, value)  (pin > PINS_MCP_OFFSET) ? pins::mcp.digitalWrite(pin - PINS_MCP_OFFSET, value) : digitalWrite(pin, value)

#define digitalReadExt(pin) (pin > PINS_MCP_OFFSET ? pins::mcp.digitalRead(pin - PINS_MCP_OFFSET) : digitalRead(pin))

void setup() {
    if (!mcp.begin_I2C()) {
        Serial.println("Failed to find MCP23X17");
        while (1)
            ;
    }
}

}  // namespace pins