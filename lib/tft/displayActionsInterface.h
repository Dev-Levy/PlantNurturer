#pragma once
#include <Arduino.h>
// #include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

class IDisplayActions
{
public:
    virtual ~IDisplayActions() = default;

    virtual void begin() = 0;
    virtual void clearScreen() = 0;
    virtual void setCursor(int x, int y) = 0;
    virtual void setTextSize(uint8_t size) = 0;
    virtual void setTextColor(uint16_t color) = 0;
    virtual void setTextColor(uint16_t color, uint16_t bgColor) = 0;

    virtual void print(const char *text) = 0;
    virtual void print(const __FlashStringHelper *text) = 0;
    virtual void print(const int value) = 0;

    virtual void println(const char *text) = 0;
    virtual void println(const __FlashStringHelper *text) = 0;
    virtual void println(const int value) = 0;

    virtual Adafruit_ST7735 &getTFT() = 0;
};