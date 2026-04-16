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

    inline void setCursor(uint8_t x, uint8_t y) { tft.setCursor(x, y); }
    inline void setTextSize(uint8_t size) { tft.setTextSize(size); }
    inline void setTextColor(uint16_t color) { tft.setTextColor(color); }
    inline void setTextColor(uint16_t color, uint16_t bgColor) { tft.setTextColor(color, bgColor); }

    inline void fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color)
    {
        tft.fillRect(x, y, width, height, color);
    }

    inline void drawFastHLine(int16_t x, int16_t y, int16_t width, uint16_t color)
    {
        tft.drawFastHLine(x, y, width, color);
    }
    inline void print(const char *text) { tft.print(text); }
    inline void print(const __FlashStringHelper *text) { tft.print(text); }
    inline void print(const int16_t value) { tft.print(value); }

    inline void println(const char *text) { tft.println(text); }
    inline void println(const __FlashStringHelper *text) { tft.println(text); }
    inline void println(const int16_t value) { tft.println(value); }

    inline int16_t getWidth() { return tft.width(); }
    inline int16_t getHeight() { return tft.height(); }

private:
    Adafruit_ST7735 tft;
};