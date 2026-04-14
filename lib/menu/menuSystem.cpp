#include <avr/pgmspace.h>
// #include <EEPROM.h>

#include "MenuSystem.h"

uint8_t activePlantIndex = 0;
static unsigned long lastMenuRefresh = 0;
const MenuPage *selectedPlantPages[3] = {nullptr, nullptr, nullptr};
MenuSystem *globalMenuPtr = nullptr;

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
    globalMenuPtr = this;
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

        if (currentPage == &plantsPage && currentCursor < PLANTS_PAGE_ITEMS - 1)
        {
            activePlantIndex = currentCursor;

            if (selectedPlantPages[activePlantIndex] == nullptr)
            {
                currentPage = &selectPlantPage;
                currentCursor = 0;
            }
            else
            {
                currentPage = selectedPlantPages[activePlantIndex];
                currentCursor = 0;
            }
        }
        else if (targetPage)
        {
            currentPage = targetPage;
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

    if (currentPage == &sensorPage && millis() - lastMenuRefresh > 2000)
    {
        drawMenuItems();
        lastMenuRefresh = millis();
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

    Serial.println(F("Printing items:------------------------------"));
    for (uint8_t i = 0; i < count; i++)
    {
        const char *label = (const char *)pgm_read_ptr(&(items[i].label));

        Serial.println((const __FlashStringHelper *)label);

        if (currentPage == &plantsPage && i < PLANTS_PAGE_ITEMS - 1)
        {
            if (selectedPlantPages[i] != nullptr)
            {
                Serial.print(F("Label change: "));
                label = (const char *)pgm_read_ptr(&(selectedPlantPages[i]->title));

                Serial.println((const __FlashStringHelper *)label);
            }
            else
            {
                Serial.print(F("selectedPlantPages["));
                Serial.print(i);
                Serial.println(F("] is nullptr"));
            }
        }

        drawItem(28 + (i * 16), label, i == currentCursor);
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
