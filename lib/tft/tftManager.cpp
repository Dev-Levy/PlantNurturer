#include "displayManager.h"

DisplayManager::DisplayManager()
    : tft(TFT_CS, TFT_DC, TFT_RST) // initialize Adafruit_ST7735 object
{
}

void DisplayManager::begin()
{
    tft.initR(INITR_BLACKTAB);
    tft.setRotation(3);
    tft.fillScreen(ST77XX_BLACK);
}

void DisplayManager::clearScreen()
{
    tft.fillScreen(ST77XX_BLACK);
}

void DisplayManager::printTitle(const __FlashStringHelper *title)
{
    fillRectangle(0, 0, getWidth(), 20, ST77XX_BLUE);
    setCursor(4, 3);
    setTextColor(ST77XX_WHITE);
    setTextSize(2);
    print(title);
    drawFastHLine(0, 21, getWidth(), ST77XX_WHITE);
}

void DisplayManager::setCursor(uint8_t x, uint8_t y)
{
    tft.setCursor(x, y);
}

void DisplayManager::setTextColor(uint16_t color)
{
    tft.setTextColor(color);
}

void DisplayManager::setTextColor(uint16_t color, uint16_t bgColor)
{
    tft.setTextColor(color, bgColor);
}

void DisplayManager::fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color)
{
    tft.fillRect(x, y, width, height, color);
}

void DisplayManager::drawFastHLine(int16_t x, int16_t y, int16_t width, uint16_t color)
{
    tft.drawFastHLine(x, y, width, color);
}

void DisplayManager::setTextSize(uint8_t size)
{
    tft.setTextSize(size);
}

void DisplayManager::print(const char *text)
{
    tft.print(text);
}

void DisplayManager::print(const __FlashStringHelper *text)
{
    tft.print(text);
}

void DisplayManager::print(const int16_t value)
{
    tft.print(value);
}

void DisplayManager::println(const char *text)
{
    tft.println(text);
}

void DisplayManager::println(const __FlashStringHelper *text)
{
    tft.println(text);
}

void DisplayManager::println(const int16_t value)
{
    tft.println(value);
}

int16_t DisplayManager::getWidth()
{
    return tft.width();
}

int16_t DisplayManager::getHeight()
{
    return tft.height();
}