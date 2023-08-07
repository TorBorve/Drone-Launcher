#pragma once

#include <Adafruit_MCP23X17.h>

///////////////////////////
// MCP23X17 Pin extensions
///////////////////////////

namespace DroneLauncher {

namespace pins {

extern Adafruit_MCP23X17 mcp;

void setup();

}  // namespace pins

#define PINS_MCP_OFFSET 64

#define PINS_MCP_ADDR 0x21

#define pinModeExt(pin, mode) (pin >= PINS_MCP_OFFSET) ? pins::mcp.pinMode(pin - PINS_MCP_OFFSET, mode) : pinMode(pin, mode)

#define digitalWriteExt(pin, value)  (pin >= PINS_MCP_OFFSET) ? pins::mcp.digitalWrite(pin - PINS_MCP_OFFSET, value) : digitalWrite(pin, value)

#define digitalReadExt(pin) (pin >= PINS_MCP_OFFSET ? pins::mcp.digitalRead(pin - PINS_MCP_OFFSET) : digitalRead(pin))


///////////////////////////
// Pin Mapping
///////////////////////////

#define PIN_MY_SPI_CS 0
#define PIN_MY_SPI_MISO 1
#define PIN_F1R1_TRIGGER_SERVO 2
#define PIN_F1R2_TRIGGER_SERVO 3
#define PIN_F1R1_SAFETY_SERVO 4
#define PIN_F1R2_SAFETY_SERVO 5
#define PIN_F2R1_TRIGGER_SERVO 6
#define PIN_F2R2_TRIGGER_SERVO 7
#define PIN_F2R1_SAFETY_SERVO 8
#define PIN_F2R2_SAFETY_SERVO 9
#define PIN_F3R1_TRIGGER_SERVO 10
#define PIN_F3R2_TRIGGER_SERVO 11
#define PIN_F3R1_SAFETY_SERVO 12
#define PIN_F3R2_SAFETY_SERVO 24
#define PIN_BAT_LEDS 25
#define PIN_MY_SPI_MOSI 26
#define PIN_MY_SPI_SCK 27
#define PIN_MENU_BACK 28
#define PIN_MENU_UP 29
#define PIN_MENU_DOWN 30
#define PIN_MENU_SELECT 31
#define PIN_ARM_SWITCH 32

#define PIN_BAT_MEAS 23
#define PIN_GPS_RX 21
#define PIN_GPS_TX 20
#define PIN_SCL 19
#define PIN_SDA 18
#define PIN_RESET_MCP 14
#define PIN_LED_DBG 37
#define PIN_LED_ALIVE 36
#define PIN_F3R2_SAFETY_SWITCH 35
#define PIN_F3R2_REAR_SWITCH 34
#define PIN_DRONE_LEDS 33

#define PIN_F2R1_REAR_SWITCH (0 + PINS_MCP_OFFSET)
#define PIN_F2R2_FRONT_SWITCH (1 + PINS_MCP_OFFSET)
#define PIN_F2R2_SAFETY_SWITCH (2 + PINS_MCP_OFFSET)
#define PIN_F2R2_REAR_SWITCH (3 + PINS_MCP_OFFSET)
#define PIN_F3R1_FRONT_SWITCH (4 + PINS_MCP_OFFSET)
#define PIN_F3R1_SAFETY_SWITCH (5 + PINS_MCP_OFFSET)
#define PIN_F3R1_REAR_SWITCH (6 + PINS_MCP_OFFSET)
#define PIN_F3R2_FRONT_SWITCH (7 + PINS_MCP_OFFSET)
#define PIN_F2R1_SAFETY_SWITCH (8 + PINS_MCP_OFFSET)
#define PIN_F2R1_FRONT_SWITCH (9 + PINS_MCP_OFFSET)
#define PIN_F1R2_REAR_SWITCH (10 + PINS_MCP_OFFSET)
#define PIN_F1R2_SAFETY_SWITCH (11 + PINS_MCP_OFFSET)
#define PIN_F1R2_FRONT_SWITCH (12 + PINS_MCP_OFFSET)
#define PIN_F1R1_REAR_SWITCH (13 + PINS_MCP_OFFSET)
#define PIN_F1R1_SAFETY_SWITCH (14 + PINS_MCP_OFFSET)
#define PIN_F1R1_FRONT_SWITCH (15 + PINS_MCP_OFFSET)

}  // namespace DroneLauncher
