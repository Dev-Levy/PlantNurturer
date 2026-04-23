#include <avr/pgmspace.h>
#include <EEPROM.h>
#include "MenuSystem.h"

MenuSystem *globalMenuPtr = nullptr;

MenuSystem::MenuSystem(TimeManager &time, DisplayManager &display, SensorManager &sensor, ActuatorManager &actuator, PlantManager &plant)
    : time(time), display(display),
      sensor(sensor), actuator(actuator),
      plant(plant)
{
}

void MenuSystem::begin()
{
    loadSettings();

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

void MenuSystem::refresh()
{
    // refresh every minute (with some buffer)
    if (currentPage == &homePage && millis() - lastHomeRefresh > 60500)
    {
        time.updateTime();
        drawTimeRow();
        lastHomeRefresh = millis();
    }

    // refresh every 2 seconds
    if (currentPage == &sensorPage && millis() - lastSensorRefresh > 2000)
    {
        currentReading = sensor.readAll();
        drawSensorPageMenuItems();
        lastSensorRefresh = millis();
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
    else if (currentPage == &tomatoPage || currentPage == &chiliPage || currentPage == &mintPage || currentPage == &basilPage)
    {
        drawPlantPageMenuItems();
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

void MenuSystem::saveSettings()
{
    Serial.println(F("Saving"));

    SettingsSave data;
    data.magic = 0xABCD;

    data.mainPlantIndex = mainPlantIndex;
    time.getGrowthStartTime(data.growthStartTime);

    for (uint8_t i = 0; i < PLANT_COUNT; i++)
    {
        data.configs[i] = storedConfigs[i];

        if (selectedPlantPages[i] == &tomatoPage)
            data.plantIds[i] = TOMATO_CONFIG_ID;
        else if (selectedPlantPages[i] == &chiliPage)
            data.plantIds[i] = CHILI_CONFIG_ID;
        else if (selectedPlantPages[i] == &mintPage)
            data.plantIds[i] = MINT_CONFIG_ID;
        else if (selectedPlantPages[i] == &basilPage)
            data.plantIds[i] = BASIL_CONFIG_ID;
        else
            data.plantIds[i] = -1;
    }

    EEPROM.put(EEPROM_ADDR, data);
}

void MenuSystem::loadSettings()
{
    Serial.println(F("Loading"));

    SettingsSave data;
    EEPROM.get(EEPROM_ADDR, data);

    if (data.magic != 0xABCD)
        return;

    mainPlantIndex = data.mainPlantIndex;
    time.setGrowthStartTime(data.growthStartTime);

    for (uint8_t i = 0; i < PLANT_COUNT; i++)
    {
        storedConfigs[i] = data.configs[i];

        switch (data.plantIds[i])
        {
        case TOMATO_CONFIG_ID:
            selectedPlantPages[i] = &tomatoPage;
            break;
        case CHILI_CONFIG_ID:
            selectedPlantPages[i] = &chiliPage;
            break;
        case MINT_CONFIG_ID:
            selectedPlantPages[i] = &mintPage;
            break;
        case BASIL_CONFIG_ID:
            selectedPlantPages[i] = &basilPage;
            break;
        default:
            selectedPlantPages[i] = nullptr;
            break;
        }
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
    print2Digits(time.getDay());
    display.print(F(". | "));
    print2Digits(time.getHour());
    display.print(F(":"));
    print2Digits(time.getMinute());
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

void MenuSystem::drawPlantPageMenuItems()
{
    const uint8_t count = pgm_read_byte(&(currentPage->itemCount));
    const MenuItem *items = (const MenuItem *)pgm_read_ptr(&(currentPage->items));

    const PlantConfig &cfg = storedConfigs[activePlantIndex];

    uint8_t startY = 2 * LINE_HEIGHT;
    uint8_t topIndex = (currentCursor / MAX_VISIBLE) * MAX_VISIBLE;

    for (uint8_t i = 0; i < MAX_VISIBLE; i++)
    {

        uint8_t itemIndex = topIndex + i;
        uint8_t yPos = startY + (i * LINE_HEIGHT);
        bool isSelected = (currentCursor == itemIndex);

        if (itemIndex < count)
        {
            auto *label = (const __FlashStringHelper *)pgm_read_ptr(&(items[itemIndex].label));

            setItemDrawingProps(isSelected, yPos);
            display.print(label);

            switch (itemIndex)
            {
            case 0: // Sunny Hours
                display.print(cfg.sunnyHours);
                display.print(F(" h"));
                break;
            case 1: // Water limit
                display.print(cfg.waterLimit);
                display.print(F(" %"));
                break;
            case 2: // Water Ml
                display.print(cfg.waterMl);
                display.print(F(" ml"));
                break;
            case 3: // Min Temp
                display.print(cfg.minTemp / 10);
                display.print(F("."));
                display.print(cfg.minTemp % 10 < 0 ? -cfg.minTemp % 10 : cfg.minTemp % 10);
                display.print(F(" C"));
                break;
            case 4: // Max Temp
                display.print(cfg.maxTemp / 10);
                display.print(F("."));
                display.print(cfg.maxTemp % 10 < 0 ? -cfg.maxTemp % 10 : cfg.maxTemp % 10);
                display.print(F(" C"));
                break;
            case 5: // Min humidity
                display.print(cfg.minHumi / 10);
                display.print(F("."));
                display.print(cfg.minHumi % 10);
                display.print(F(" %"));
                break;
            case 6: // Max humidity
                display.print(cfg.maxHumi / 10);
                display.print(F("."));
                display.print(cfg.maxHumi % 10);
                display.print(F(" %"));
                break;
            default:
                // A Start, Remove, Back
                break;
            }
        }
        else
        {
            display.fillRectangle(0, yPos - PADDING, display.getWidth(), LINE_HEIGHT, ST77XX_BLACK);
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

    uint8_t startY = 2 * LINE_HEIGHT;
    uint8_t topIndex = (currentCursor / MAX_VISIBLE) * MAX_VISIBLE;

    for (uint8_t i = 0; i < MAX_VISIBLE; i++)
    {
        uint8_t itemIndex = topIndex + i;
        uint8_t yPos = startY + (i * LINE_HEIGHT);

        if (itemIndex < count)
        {
            bool isSelected = (currentCursor == itemIndex);
            auto *label = (const __FlashStringHelper *)pgm_read_ptr(&(items[itemIndex].label));

            setItemDrawingProps(isSelected, yPos);
            display.print(label);
        }
        else
        {
            display.fillRectangle(0, yPos - PADDING, display.getWidth(), LINE_HEIGHT, ST77XX_BLACK);
        }
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
        display.print(data.lightLux);
        display.print(F(" lux"));
        break;
    case 2:
        display.print(data.airHumidity / 10);
        display.print(F("."));
        display.print(data.airHumidity % 10 < 0 ? -data.airHumidity % 10 : data.airHumidity % 10);
        display.print(F(" %"));
        break;
    case 3:
        display.print(data.airTemp / 10);
        display.print(F("."));
        display.print(data.airTemp % 10 < 0 ? -data.airTemp % 10 : data.airTemp % 10);
        display.print(F(" C"));
        break;
    case 4:
        display.print(data.soilMoisture / 10);
        display.print(F("."));
        display.print(data.soilMoisture % 10 < 0 ? -data.soilMoisture % 10 : data.soilMoisture % 10);
        display.print(F(" %"));
        break;
    }
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

void MenuSystem::print2Digits(uint8_t value)
{
    if (value < 10)
    {
        display.print(F("0"));
    }
    display.print(value);
}
