#pragma once
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>
#include <config.h>

#include "displayActionsInterface.h"

class TFTManager : public IDisplayActions
{
public:
    TFTManager();

    void begin() override;
    void clearScreen() override;

    void printTitle(const __FlashStringHelper *text);

    void setCursor(uint8_t x, uint8_t y) override;
    void setTextSize(uint8_t size) override;
    void setTextColor(uint16_t color) override;
    void setTextColor(uint16_t color, uint16_t bgColor) override;
    void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color);
    void drawFastHLine(int16_t x, int16_t y, int16_t width, uint16_t color);

    void print(const char *text) override;
    void print(const __FlashStringHelper *text) override;
    void print(const int16_t value) override;

    void println(const char *text) override;
    void println(const __FlashStringHelper *text) override;
    void println(const int16_t value) override;

    int16_t getWidth() override;
    int16_t getHeight() override;

    Adafruit_ST7735 &getTFT() override;

private:
    Adafruit_ST7735 tft;
};