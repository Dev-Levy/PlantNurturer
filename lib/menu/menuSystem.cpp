#include <avr/pgmspace.h>
// #include <EEPROM.h>

#include "MenuSystem.h"

static unsigned long lastMenuRefresh = 0;
MenuSystem *globalMenuPtr = nullptr;

MenuSystem::MenuSystem(TimeManager &time, DisplayManager &display, SensorManager &sensor, ActuatorManager &actuator)
    : time(time), display(display),
      sensor(sensor), actuator(actuator)
{
}

void MenuSystem::begin()
{
    globalMenuPtr = this;
    currentPage = &homePage;
    time.updateTime();
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
    currentReading = sensor.readAll();

    if (currentPage == &sensorPage && millis() - lastMenuRefresh > 2000)
    {
        drawSensorPageMenuItems();
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
        drawTimeRow();
        drawGrowingRow();
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

void MenuSystem::drawTimeRow()
{
    int yPos = 2 * LINE_HEIGHT;

    display.fillRectangle(0, yPos - PADDING, display.getWidth(), LINE_HEIGHT, ST77XX_BLACK);
    display.setTextColor(ST77XX_WHITE);
    display.setCursor(4, yPos);
    display.setTextSize(1);

    display.print(getMonthName(time.getMonth()));
    display.print(F(" "));
    display.print(time.getDay());
    display.print(F(". | "));
    display.print(time.getHour());
    display.print(F(":"));
    display.print(time.getMinute());
    display.print(F(" | Week "));
    display.print(time.getGrowthWeek(time.getUnixNow()));
}

void MenuSystem::drawGrowingRow()
{
    int yPos = 3 * LINE_HEIGHT;

    display.fillRectangle(0, yPos - PADDING, display.getWidth(), LINE_HEIGHT, ST77XX_BLACK);
    display.setTextColor(ST77XX_WHITE);
    display.setCursor(4, yPos);
    display.setTextSize(1);

    if (selectedPlantPages[mainPlantIndex] != nullptr)
    {
        auto plantName = (const __FlashStringHelper *)pgm_read_ptr(&(selectedPlantPages[mainPlantIndex]->title));
        display.print(F("Growing: "));
        display.print(plantName);
    }
    else
    {
        display.print(F("Growing: ---"));
    }
}

void MenuSystem::drawHomePageMenuItems()
{
    const uint8_t count = pgm_read_byte(&(currentPage->itemCount));
    const MenuItem *items = (const MenuItem *)pgm_read_ptr(&(currentPage->items));

    for (uint8_t i = 0; i < count; i++)
    {
        bool isSelected = (i == currentCursor);
        int yPos = 4 * LINE_HEIGHT + (i * (LINE_HEIGHT + PADDING));

        auto *label = (const __FlashStringHelper *)pgm_read_ptr(&(items[i].label));

        setItemDrawingProps(isSelected, yPos);
        display.print(label);
    }
}

void MenuSystem::drawPlantsPageMenuItems()
{
    const uint8_t count = pgm_read_byte(&(currentPage->itemCount));
    const MenuItem *items = (const MenuItem *)pgm_read_ptr(&(currentPage->items));

    for (uint8_t i = 0; i < count; i++)
    {
        bool isSelected = (i == currentCursor);
        int yPos = 2 * LINE_HEIGHT + (i * (LINE_HEIGHT + PADDING));
        setItemDrawingProps(isSelected, yPos);

        if (i < PLANTS_PAGE_ITEMS - 1)
        {
            if (selectedPlantPages[i] != nullptr)
            {
                auto *title = (const __FlashStringHelper *)pgm_read_ptr(&(selectedPlantPages[i]->title));
                display.print(title);

                if (i == mainPlantIndex)
                {
                    display.setTextColor(isSelected ? ST77XX_BLACK : ST77XX_YELLOW);
                    display.print(F(" *"));
                }
            }
            else
            {
                auto *label = (const __FlashStringHelper *)pgm_read_ptr(&(items[i].label));
                display.print(label);
            }
        }
        else
        {
            auto *label = (const __FlashStringHelper *)pgm_read_ptr(&(items[i].label));
            display.print(label);
        }
    }
}

void MenuSystem::drawSensorPageMenuItems()
{
    const uint8_t count = pgm_read_byte(&(currentPage->itemCount));
    const MenuItem *items = (const MenuItem *)pgm_read_ptr(&(currentPage->items));

    for (uint8_t i = 0; i < count; i++)
    {
        bool isSelected = (i == currentCursor);
        int yPos = 2 * LINE_HEIGHT + (i * (LINE_HEIGHT + PADDING));
        auto *label = (const __FlashStringHelper *)pgm_read_ptr(&(items[i].label));

        if (i < SENSORS_PAGE_ITEMS - 1)
        {
            drawSensorPageMenuItem(i, yPos, isSelected, currentReading, label);
        }
        else
        {
            setItemDrawingProps(isSelected, yPos);
            display.print(label);
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
        int yPos = 2 * LINE_HEIGHT + (i * (LINE_HEIGHT + PADDING));

        auto *label = (const __FlashStringHelper *)pgm_read_ptr(&(items[i].label));

        setItemDrawingProps(isSelected, yPos);
        display.print(label);
    }
}

void MenuSystem::drawSensorPageMenuItem(uint8_t index, uint8_t y, bool isSelected, const SensorReadings &data, const __FlashStringHelper *label)
{
    setItemDrawingProps(isSelected, y);
    display.print(label);

    switch (index)
    {
    case 0:
        display.print(data.light ? F("LIGHT") : F("DARK"));
        break;
    case 1:
        display.print(data.airTemp / 10);
        display.print(F("."));
        display.print(data.airTemp % 10 < 0 ? -data.airTemp % 10 : data.airTemp % 10);
        display.print(F("C"));
        break;
    case 2:
        display.print(data.airHumidity / 10);
        display.print(F("."));
        display.print(data.airHumidity % 10 < 0 ? -data.airHumidity % 10 : data.airHumidity % 10);
        display.print(F("%"));
        break;
    case 3:
        display.print(data.soilTemp / 10);
        display.print(F("."));
        display.print(data.soilTemp % 10 < 0 ? -data.soilTemp % 10 : data.soilTemp % 10);
        display.print(F("C"));
        break;
    case 4:
        display.print(data.soilMoisture / 10);
        display.print(F("."));
        display.print(data.soilMoisture % 10 < 0 ? -data.soilMoisture % 10 : data.soilMoisture % 10);
        display.print(F("%"));
        break;
    }
}

const char *MenuSystem::getMonthName(uint8_t month)
{
    static const char *months[] = {
        "Inv",
        "Jan", "Feb", "Mar",
        "Apr", "May", "Jun",
        "Jul", "Aug", "Sep",
        "Oct", "Nov", "Dec"};

    if (month > 12)
    {
        return months[0];
    }

    return months[month];
}

void MenuSystem::setItemDrawingProps(bool isSelected, uint8_t y)
{
    if (isSelected)
    {
        display.fillRectangle(0, y - 2, display.getWidth(), LINE_HEIGHT, ST77XX_WHITE);
        display.setTextColor(ST77XX_BLACK);
        display.setCursor(2, y);
        display.setTextSize(1);
        display.print(F("> "));
    }
    else
    {
        display.fillRectangle(0, y - 2, display.getWidth(), LINE_HEIGHT, ST77XX_BLACK);
        display.setTextColor(ST77XX_WHITE);
    }
    display.setCursor(10, y);
    display.setTextSize(1);
}
