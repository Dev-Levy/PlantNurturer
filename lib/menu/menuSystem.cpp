#include <avr/pgmspace.h>
// #include <EEPROM.h>

#include "MenuSystem.h"

// static members
MenuSystem *MenuSystem::globalMenuPtr = nullptr;
uint8_t MenuSystem::currentCursor = 0;
const MenuPage *MenuSystem::currentPage = nullptr;

uint8_t MenuSystem::mainPlantIndex = 0;
uint8_t MenuSystem::activePlantIndex = 0;
const MenuPage *MenuSystem::selectedPlantPages[3] = {nullptr, nullptr, nullptr};

static unsigned long lastMenuRefresh = 0;

MenuSystem::MenuSystem(IDisplayActions &display, ISensorActions &sensorActions, IActuatorActions &actuatorActions)
    : display(display), sensorActions(sensorActions), actuatorActions(actuatorActions),
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
    case KeyPress::UP:
        currentCursor = (currentCursor == 0) ? count - 1 : currentCursor - 1;
        break;

    case KeyPress::DOWN:
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

    if (currentPage == &homePage)
    {
        drawHomePageMenuItems();
    }
    else if (currentPage == &plantsPage)
    {
        drawPlantsPageMenuItems();
    }
    else if (currentPage == &sensorPage)
    {
        drawSensorPageMenuItems();
    }
    else
    {
        drawMenuItems();
    }
}

void MenuSystem::drawHomePageMenuItems()
{
    const uint8_t count = pgm_read_byte(&(currentPage->itemCount));
    const MenuItem *items = (const MenuItem *)pgm_read_ptr(&(currentPage->items));
    static char dynamicBuffer[32];

    for (uint8_t i = 0; i < count; i++)
    {
        bool isSelected = (i == currentCursor);
        int yPos = 28 + (i * 16);

        if (i == 0)
        {
            if (selectedPlantPages[mainPlantIndex] != nullptr)
            {
                auto plantName = (const char *)pgm_read_ptr(&(selectedPlantPages[mainPlantIndex]->title));
                strcpy_P(dynamicBuffer, (const char *)F("Growing: "));
                strcat_P(dynamicBuffer, plantName);
            }
            else
            {
                strcpy_P(dynamicBuffer, (const char *)F("Growing: ---"));
            }
            drawItem(yPos, dynamicBuffer, isSelected); // RAM
        }
        else
        {
            auto *label = (const __FlashStringHelper *)pgm_read_ptr(&(items[i].label));
            drawItem(yPos, label, isSelected); // flash
        }
    }
}

void MenuSystem::drawPlantsPageMenuItems()
{
    const uint8_t count = pgm_read_byte(&(currentPage->itemCount));
    const MenuItem *items = (const MenuItem *)pgm_read_ptr(&(currentPage->items));

    for (uint8_t i = 0; i < count; i++)
    {
        bool isSelected = (i == currentCursor);
        int yPos = 28 + (i * 16);

        if (i < PLANTS_PAGE_ITEMS - 1)
        {
            if (selectedPlantPages[i] != nullptr)
            {
                auto *title = (const __FlashStringHelper *)pgm_read_ptr(&(selectedPlantPages[i]->title));
                drawItem(yPos, title, isSelected); // flash
                if (i == mainPlantIndex)
                {
                    display.setTextColor(isSelected ? ST77XX_BLACK : ST77XX_YELLOW);
                    display.print(F(" *"));
                }
            }
            else
            {
                auto *label = (const __FlashStringHelper *)pgm_read_ptr(&(items[i].label));
                drawItem(yPos, label, isSelected); // flash
            }
        }
        else
        {
            auto *label = (const __FlashStringHelper *)pgm_read_ptr(&(items[i].label));
            drawItem(yPos, label, isSelected); // flash
        }
    }
}

void MenuSystem::drawSensorPageMenuItems()
{
    const uint8_t count = pgm_read_byte(&(currentPage->itemCount));
    const MenuItem *items = (const MenuItem *)pgm_read_ptr(&(currentPage->items));
    static char dynamicBuffer[32];

    for (uint8_t i = 0; i < count; i++)
    {
        bool isSelected = (i == currentCursor);
        int yPos = 28 + (i * 16);

        if (i < SENSORS_PAGE_ITEMS - 1)
        {
            getSensorString(i, dynamicBuffer);
            drawItem(yPos, dynamicBuffer, isSelected); // RAM
        }
        else
        {
            auto *label = (const __FlashStringHelper *)pgm_read_ptr(&(items[i].label));
            drawItem(yPos, label, isSelected); // flash
        }
    }
}

void MenuSystem::drawMenuItems()
{
    const uint8_t count = pgm_read_byte(&(currentPage->itemCount));
    const MenuItem *items = (const MenuItem *)pgm_read_ptr(&(currentPage->items));

    for (uint8_t i = 0; i < count; i++)
    {
        bool isSelected = (i == currentCursor);
        int yPos = 28 + (i * 16);

        auto *label = (const __FlashStringHelper *)pgm_read_ptr(&(items[i].label));
        drawItem(yPos, label, isSelected); // flash
    }
}

void MenuSystem::drawItem(int y, const __FlashStringHelper *text, bool selected)
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
    display.print(text);
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
    display.print(text);
}

void MenuSystem::getSensorString(uint8_t index, char *buffer)
{
    const MenuItem *items = (const MenuItem *)pgm_read_ptr(&(sensorPage.items));
    const char *label = (const char *)pgm_read_ptr(&(items[index].label));

    strcpy_P(buffer, label);
    char *valuePos = buffer + strlen(buffer);

    switch (index)
    {
    case 0:
        dtostrf(currentReadings.light, 4, 0, valuePos);
        break;
    case 1:
        dtostrf(currentReadings.airTemp, 4, 1, valuePos);
        strcat(buffer, "C");
        break;
    case 2:
        dtostrf(currentReadings.airHumidity, 4, 1, valuePos);
        strcat(buffer, "%");
        break;
    case 3:
        dtostrf(currentReadings.soilMoisture, 4, 0, valuePos);
        strcat(buffer, "%");
        break;
    case 4:
        dtostrf(currentReadings.soilTemp, 4, 1, valuePos);
        strcat(buffer, "C");
        break;
    }
}
