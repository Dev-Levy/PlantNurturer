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
    void setCursor(int x, int y) override;
    void setTextSize(uint8_t size) override;
    void setTextColor(uint16_t color) override;
    void setTextColor(uint16_t color, uint16_t bgColor) override;

    void print(const char *text) override;
    void print(const __FlashStringHelper *text) override;
    void print(const int value) override;

    void println(const char *text) override;
    void println(const __FlashStringHelper *text) override;
    void println(const int value) override;

    Adafruit_ST7735 &getTFT() override;

private:
    Adafruit_ST7735 tft;
};