#include "TFTManager.h"

TFTManager::TFTManager()
    : tft(TFT_CS, TFT_DC, TFT_RST) // initialize Adafruit_ST7735 object
{
}

void TFTManager::begin()
{
    tft.initR(INITR_BLACKTAB);
    tft.setRotation(1);
    tft.fillScreen(ST77XX_BLACK);
}

void TFTManager::clearScreen()
{
    tft.fillScreen(ST77XX_BLACK);
}

void TFTManager::setCursor(int x, int y)
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

void TFTManager::setTextSize(uint8_t size)
{
    tft.setTextSize(size);
}

void TFTManager::print(const char *text)
{
    tft.print(text);
}

void TFTManager::print(const int value)
{
    tft.print(value);
}

void TFTManager::println(const char *text)
{
    tft.println(text);
}
void TFTManager::println(const int value)
{
    tft.println(value);
}

Adafruit_ST7735 &TFTManager::getTFT()
{
    return tft;
}