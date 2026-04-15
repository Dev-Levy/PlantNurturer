#include "TFTManager.h"

TFTManager::TFTManager()
    : tft(TFT_CS, TFT_DC, TFT_RST) // initialize Adafruit_ST7735 object
{
}

void TFTManager::begin()
{
    tft.initR(INITR_BLACKTAB);
    tft.setRotation(3);
    tft.fillScreen(ST77XX_BLACK);
}

void TFTManager::clearScreen()
{
    tft.fillScreen(ST77XX_BLACK);
}

void TFTManager::printTitle(const __FlashStringHelper *title)
{
    fillRectangle(0, 0, getWidth(), 20, ST77XX_BLUE);
    setCursor(4, 3);
    setTextColor(ST77XX_WHITE);
    setTextSize(2);
    print(title);
    drawFastHLine(0, 21, getWidth(), ST77XX_WHITE);
}

void TFTManager::setCursor(uint8_t x, uint8_t y)
{
    tft.setCursor(x, y);
}

void TFTManager::setTextColor(uint16_t color)
{
    tft.setTextColor(color);
}

void TFTManager::setTextColor(uint16_t color, uint16_t bgColor)
{
    tft.setTextColor(color, bgColor);
}

void TFTManager::fillRectangle(int16_t x, int16_t y, int16_t width, int16_t height, uint16_t color)
{
    tft.fillRect(x, y, width, height, color);
}

void TFTManager::drawFastHLine(int16_t x, int16_t y, int16_t width, uint16_t color)
{
    tft.drawFastHLine(x, y, width, color);
}

void TFTManager::setTextSize(uint8_t size)
{
    tft.setTextSize(size);
}

void TFTManager::print(const char *text)
{
    tft.print(text);
}

void TFTManager::print(const __FlashStringHelper *text)
{
    tft.print(text);
}

void TFTManager::print(const int16_t value)
{
    tft.print(value);
}

void TFTManager::println(const char *text)
{
    tft.println(text);
}

void TFTManager::println(const __FlashStringHelper *text)
{
    tft.println(text);
}

void TFTManager::println(const int16_t value)
{
    tft.println(value);
}

int16_t TFTManager::getWidth()
{
    return tft.width();
}

int16_t TFTManager::getHeight()
{
    return tft.height();
}

Adafruit_ST7735 &TFTManager::getTFT()
{
    return tft;
}