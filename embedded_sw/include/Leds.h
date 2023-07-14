#pragma once

#include <FastLED.h>
#include <stdint.h>

////////////////////////////////////////
// BaseLed
////////////////////////////////////////

class BaseLed {
   public:
    enum class Mode : uint8_t { OFF,
                                ON,
                                BLINK };
    BaseLed(Mode mode = Mode::OFF);
    void update(uint32_t now);
    void setMode(Mode mode);
    Mode getMode() const { return _mode; };

   protected:
    virtual void writeToLed() = 0;
    Mode _mode;
    bool _isOn;
    uint32_t _prevBlink;
};

////////////////////////////////////////
// Led
////////////////////////////////////////

class Led : public BaseLed {
   public:
    Led(uint8_t pin, Mode mode = Mode::OFF);

   protected:
    virtual void writeToLed() override;
    uint8_t _pin;
};

////////////////////////////////////////
// RGBLed
////////////////////////////////////////

class RGBLed : public BaseLed {
   public:
    RGBLed(CRGB& color, Mode mode = Mode::OFF);
    void setColor(const CRGB& color) { _color = color; };

   protected:
    virtual void writeToLed() override;
    CRGB& _actualColor;  // reference to the actual color
    CRGB _color;          // the color we want to set.
};