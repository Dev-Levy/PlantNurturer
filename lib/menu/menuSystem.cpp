#include <avr/pgmspace.h>
// #include <EEPROM.h>

#include "MenuSystem.h"

// static char typesPlants[PLANT_TYPE_COUNT][16];

uint8_t activePlantIndex = 0;

static IActuatorActions *actuatorActionsContext = nullptr;
static MenuSystem *globalMenuPtr = nullptr;

// static char valBuffer[20];

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
    actuatorActionsContext = &actuatorActions;
    currentPage = &homePage;

    // reset EEPROM
    // savePlantData();
    // loadPlantData();
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
        drawDynamicSensorData();
        lastSerialWrite = millis();
    }
}

void MenuSystem::draw()
{
    const __FlashStringHelper *title = (const __FlashStringHelper *)pgm_read_ptr(&(currentPage->title));
    const uint8_t count = pgm_read_byte(&(currentPage->itemCount));
    const MenuItem *items = (const MenuItem *)pgm_read_ptr(&(currentPage->items));

    display.clearScreen();
    display.printTitle(title);

    for (uint8_t i = 0; i < count; i++)
    {
        const char *label = (const char *)pgm_read_ptr(&(items[i].label));

        bool isSelected = (i == currentCursor);
        drawItem(28 + (i * 16), label, isSelected);
    }
}

void MenuSystem::drawDynamicSensorData()
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

void MenuSystem::getSensorString(int index, char *buffer)
{
    switch (index)
    {
    case 0:
        sprintf(buffer, "Light: %s", currentReadings.light ? "FALSE" : "TRUE");
        break;
    case 1:
    {
        int s_whole = (int)currentReadings.soilTemp;
        int s_dec = (int)((currentReadings.soilTemp - s_whole) * 100);
        sprintf(buffer, "Soil Temp:  %d.%02d C", s_whole, s_dec);
        break;
    }
    case 2:
    {
        int h_whole = (int)currentReadings.humidity;
        int h_dec = (int)((currentReadings.humidity - h_whole) * 100);
        sprintf(buffer, "Humidity: %d.%02d%%", h_whole, h_dec);
        break;
    }
    case 3:
    {
        int a_whole = (int)currentReadings.airTemp;
        int a_dec = (int)((currentReadings.airTemp - a_whole) * 100);
        sprintf(buffer, "Air Temp:   %d.%02d C", a_whole, a_dec);
        break;
    }
    case 4:
        sprintf(buffer, "Soil Moisture: %d", currentReadings.soilMoisture);
        break;
    default:
        sprintf(buffer, "Unknown Sensor");
        break;
    }
}
