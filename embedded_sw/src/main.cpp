#include <Arduino.h>

#include "Buttons.h"
#include "LaunchUnit.h"
#include "cmath"

// #include <TeensyThreads.h>

#define SAFETY_SERVO_PIN 1
// #define SAFETY_BUTTON_PIN 3
// #define TRIGGER_BUTTON_PIN 20
#define TRIGGER_SERVO_PIN 21
// #define SAFETY_SERVO_MIN_ANGLE 163
// #define SAFETY_SERVO_MAX_ANGLE 150

#define REAR_SWITCH_PIN 3
#define SAFETY_SWITCH_PIN 4
#define FRONT_SWITCH_PIN 5

#define LED_PIN     2
#define NUM_LEDS    6

CRGB leds[NUM_LEDS];

LaunchUnit launchUnit{TRIGGER_SERVO_PIN, SAFETY_SERVO_PIN, REAR_SWITCH_PIN, SAFETY_SWITCH_PIN, FRONT_SWITCH_PIN, leds[0]};

void setup() {
    delay(1000);
    Serial.println("Setup");
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    FastLED.show();
}

void loop() {
    uint32_t now = millis();
    launchUnit.update(now);
    FastLED.show();
}