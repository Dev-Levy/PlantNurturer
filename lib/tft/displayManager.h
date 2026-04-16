#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <config.h>

class DisplayManager
{
public:
    DisplayManager();

    void begin();
    void clearScreen();

    void printTitle(const __FlashStringHelper *text);

    void setCursor(uint8_t x, uint8_t y);
    void setTextSize(uint8_t size);
    void setTextColor(uint16_t color);
    void setTextColor(uint16_t color, uint16_t bgColor);
    void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t width, uint16_t color);

    void print(const char *text);
    void print(const __FlashStringHelper *text);
    void print(const int16_t value);

    void println(const char *text);
    void println(const __FlashStringHelper *text);
    void println(const int16_t value);

    int16_t getWidth();
    int16_t getHeight();

private:
    Adafruit_ST7735 tft;
};