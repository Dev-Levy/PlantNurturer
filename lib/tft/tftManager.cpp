#include "displayManager.h"

DisplayManager::DisplayManager()
    : tft(TFT_CS, TFT_DC, TFT_RST) // initialize Adafruit_ST7735 object
{
}

void DisplayManager::begin()
{
    tft.initR(INITR_BLACKTAB);
    tft.setRotation(1);
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