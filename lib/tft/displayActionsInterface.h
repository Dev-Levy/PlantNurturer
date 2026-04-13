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

    virtual void printTitle(const __FlashStringHelper *text) = 0;

    virtual void setCursor(uint8_t x, uint8_t y) = 0;
    virtual void setTextSize(uint8_t size) = 0;
    virtual void setTextColor(uint16_t color) = 0;
    virtual void setTextColor(uint16_t color, uint16_t bgColor) = 0;
    virtual void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color) = 0;
    virtual void drawFastHLine(int16_t x, int16_t y, int16_t width, uint16_t color);

    virtual void print(const char *text) = 0;
    virtual void print(const __FlashStringHelper *text) = 0;
    virtual void print(const int16_t value) = 0;

    virtual void println(const char *text) = 0;
    virtual void println(const __FlashStringHelper *text) = 0;
    virtual void println(const int16_t value) = 0;

    virtual int16_t getWidth() = 0;
    virtual int16_t getHeight() = 0;
    virtual Adafruit_ST7735 &getTFT() = 0;
};