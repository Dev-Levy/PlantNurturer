#include <avr/pgmspace.h>
// #include <EEPROM.h>

#include "MenuSystem.h"

uint8_t activePlantIndex = 0;

MenuSystem::MenuSystem(IDisplayActions &display, ISensorActions &sensorActions, IActuatorActions &actuatorActions)
    : display(display),
      sensorActions(sensorActions),
      actuatorActions(actuatorActions),
      currentCursor(0),
      currentPage(nullptr),
      currentReadings()
{
}

void MenuSystem::begin()
{
    currentPage = &homePage;
    draw();
}

void MenuSystem::processKey(KeyPress key)
{
    uint8_t count = pgm_read_byte(&(currentPage->itemCount));

    switch (key)
    {
    case KeyPress::UP: // UP
        currentCursor = (currentCursor == 0) ? count - 1 : currentCursor - 1;
        break;

    case KeyPress::DOWN: // DOWN
        currentCursor = (currentCursor == count - 1) ? 0 : currentCursor + 1;
        break;

    case KeyPress::SELECT:
    {
        const MenuItem *items = (const MenuItem *)pgm_read_ptr(&(currentPage->items));

        const MenuPage *targetPage = (const MenuPage *)pgm_read_ptr(&(items[currentCursor].targetPage));
        void (*callback)(void *) = (void (*)(void *))pgm_read_ptr(&(items[currentCursor].callback));
        void *ctx = (void *)pgm_read_ptr(&(items[currentCursor].callbackContext));

        if (targetPage)
        {
            currentPage = targetPage;
            if (callback)
            {
                callback(ctx);
            }

            currentCursor = 0;
        }
        else if (callback)
        {
            callback(ctx);
        }
    }

    default:
        break;
    }
}

void MenuSystem::updateSensorValues()
{
    currentReadings = sensorActions.readAll();

    static unsigned long lastSerialWrite = 0;
    if (currentPage == &sensorPage && millis() - lastSerialWrite > 2000)
    {
        drawMenuItems();
        lastSerialWrite = millis();
    }
}

void MenuSystem::draw()
{
    const __FlashStringHelper *title = (const __FlashStringHelper *)pgm_read_ptr(&(currentPage->title));

    display.clearScreen();
    display.printTitle(title);

    drawMenuItems();
}

void MenuSystem::drawMenuItems()
{
    const uint8_t count = pgm_read_byte(&(currentPage->itemCount));
    const MenuItem *items = (const MenuItem *)pgm_read_ptr(&(currentPage->items));

    for (uint8_t i = 0; i < count; i++)
    {
        const char *label = (const char *)pgm_read_ptr(&(items[i].label));

        bool isSelected = (i == currentCursor);
        drawItem(28 + (i * 16), label, isSelected);
    }
}

void MenuSystem::drawItem(int y, const char *text, bool selected)
{
    uint16_t itemHeight = 14;

    if (selected)
    {
        display.fillRectangle(0, y - 2, display.getWidth(), itemHeight, ST77XX_WHITE);
        display.setTextColor(ST77XX_BLACK);
        display.setCursor(2, y);
        display.setTextSize(1);
        display.print(F("> "));
    }
    else
    {
        display.fillRectangle(0, y - 2, display.getWidth(), itemHeight, ST77XX_BLACK);
        display.setTextColor(ST77XX_WHITE);
    }

    display.setCursor(10, y);
    display.setTextSize(1);
    display.print((const __FlashStringHelper *)text);
}
