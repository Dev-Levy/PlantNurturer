#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include "config.h"

class TFTManager
{
public:
    TFTManager();

    void begin(); // initialize display
    void clearScreen();
    void setCursor(int x, int y);
    void setTextSize(uint8_t size);
    void setTextColor(uint16_t color);
    void setTextColor(uint16_t color, uint16_t bgColor);
    void print(const char *text);
    void print(const int value);
    void println(const char *text);
    void println(const int value);

    Adafruit_ST7735 &getTFT(); // access raw TFT object

private:
    Adafruit_ST7735 tft;
};