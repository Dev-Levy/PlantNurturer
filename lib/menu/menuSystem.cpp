#include <avr/pgmspace.h>
#include <EEPROM.h>

#include "MenuSystem.h"

static char typesPlants[PLANT_TYPE_COUNT][16];

int activePlantIndex = 0;
static PlantSlot slotsPlants[3] = {
    {false, &plant1Page, "(select plant)"},
    {false, &plant2Page, "(select plant)"},
    {false, &plant3Page, "(select plant)"}};

static IActuatorActions *actuatorActionsContext = nullptr;
static MenuSystem *globalMenuPtr = nullptr;

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

    // savePlantData(); // uncomment to reset saved plant data
    loadPlantData();

    setupMenuPages();
    draw();
}

void MenuSystem::processKey(int key)
{
    switch (key)
    {
    case 1: // UP
        currentCursor = (currentCursor == 0) ? currentPage->itemCount - 1 : currentCursor - 1;
        break;

    case 2: // DOWN
        currentCursor = (currentCursor == currentPage->itemCount - 1) ? 0 : currentCursor + 1;
        break;

    case 3:
    {
        MenuItem &activeItem = currentPage->items[currentCursor];

        // nav
        if (activeItem.targetPage != nullptr)
        {
            currentPage = activeItem.targetPage;
            if (activeItem.callback != nullptr)
            {
                activeItem.callback(activeItem.callbackContext);
            }
            currentCursor = 0;
        }
        // action
        else if (activeItem.callback != nullptr)
        {
            activeItem.callback(activeItem.callbackContext);
        }
        // back
        else if (currentPage->parent != nullptr)
        {
            currentPage = currentPage->parent;
            currentCursor = 0;
        }
        break;
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
    drawPage(currentPage);
}

void MenuSystem::drawPage(MenuPage *page)
{
    Serial.print(F("Drawing page: "));
    Serial.println(page->title);

    display.clearScreen();
    auto tft = display.getTFT();

    tft.fillRect(0, 0, tft.width(), 20, ST77XX_BLUE);
    tft.setCursor(4, 3);
    tft.setTextColor(ST77XX_WHITE);
    tft.setTextSize(2);
    tft.print(page->title);

    tft.drawFastHLine(0, 21, tft.width(), ST77XX_WHITE);

    for (int i = 0; i < page->itemCount; i++)
    {
        int yPos = 28 + (i * 16);

        bool isSelected = (i == currentCursor);

        if (page == &sensorPage)
        {
            char valBuffer[32];
            getSensorString(i, valBuffer);
            drawItem(yPos, valBuffer, isSelected);
        }
        else
        {
            drawItem(yPos, page->items[i].label, isSelected);
        }
    }
}

void MenuSystem::drawDynamicSensorData()
{
    for (int i = 0; i < sensorPage.itemCount; i++)
    {
        int yPos = 28 + (i * 16);
        bool isSelected = (i == currentCursor);

        char valBuffer[32];
        getSensorString(i, valBuffer);
        drawItem(yPos, valBuffer, isSelected);
    }
}

void MenuSystem::drawItem(int y, const char *text, bool selected)
{
    auto tft = display.getTFT();
    int screenWidth = tft.width();
    int itemHeight = 14;

    if (selected)
    {
        tft.fillRect(0, y - 2, screenWidth, itemHeight, ST77XX_WHITE);
        tft.setTextColor(ST77XX_BLACK);
        tft.setCursor(2, y);
        tft.setTextSize(1);
        tft.print("> ");
    }
    else
    {
        tft.fillRect(0, y - 2, screenWidth, itemHeight, ST77XX_BLACK);
        tft.setTextColor(ST77XX_WHITE);
    }

    tft.setCursor(10, y);
    tft.setTextSize(1);
    tft.print(text);
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

void MenuSystem::setupMenuPages()
{
    setupHomeMenuPage();
    setupMainMenuPage();
    setupPlantsMenuPage();
    setupPlantMenuPage();
    setupSelectPlantMenuPage();
    setupSensorDataMenuPage();
    setupActuatorControlMenuPage();
}

void MenuSystem::setupHomeMenuPage()
{
    homePage.title = "Welcome!";
    homePage.items[0] = {"Enter Main Menu", &mainPage};
    homePage.itemCount = 1;
    homePage.parent = nullptr;
}

void MenuSystem::setupMainMenuPage()
{
    mainPage.title = "Main Menu";
    mainPage.parent = &homePage;
    mainPage.items[0] = {"Plants", &plantsPage};
    mainPage.items[1] = {"Sensors", &sensorPage};
    mainPage.items[2] = {"Actuators", &actuatorPage};
    mainPage.items[3] = {"(back)", &homePage};
    mainPage.itemCount = 4;
}

void MenuSystem::setupPlantsMenuPage()
{
    plantsPage.title = "Plants";
    plantsPage.parent = &mainPage;
    for (int i = 0; i < PLANT_PAGES_COUNT; i++)
    {
        MenuItem &item = plantsPage.items[i];

        if (slotsPlants[i].isSet)
        {
            item.label = slotsPlants[i].label;
            item.targetPage = slotsPlants[i].page;
            item.callback = nullptr;
            item.callbackContext = nullptr;

            slotsPlants[i].page->title = slotsPlants[i].label;
        }
        else
        {
            item.label = slotsPlants[i].label;
            item.targetPage = &setPlantPage;
            item.callback = setActivePlantCallback;
            item.callbackContext = (void *)(intptr_t)i;
        }
    }
    plantsPage.items[PLANT_PAGES_COUNT] = {"(back)", &mainPage, nullptr, nullptr};
    plantsPage.itemCount = PLANT_PAGES_COUNT + 1;

    plant1Page.parent = &plantsPage;
    plant2Page.parent = &plantsPage;
    plant3Page.parent = &plantsPage;
}

void MenuSystem::setupPlantMenuPage()
{
}

void MenuSystem::setupSelectPlantMenuPage()
{
    setPlantPage.title = "Select Plant";
    setPlantPage.parent = &plantsPage;
    setPlantPage.itemCount = PLANT_TYPE_COUNT;

    for (int i = 0; i < PLANT_TYPE_COUNT; i++)
    {
        const char *flashName = (const char *)pgm_read_word(&(plantNames[i]));
        strncpy_P(typesPlants[i], flashName, 15);
        typesPlants[i][15] = '\0';

        setPlantPage.items[i] = {
            typesPlants[i],
            &plantsPage,
            commitSelectedPlantTypeCallback,
            (void *)(intptr_t)i};
    }
}

void MenuSystem::setupSensorDataMenuPage()
{
    sensorPage.title = "Live Data";
    sensorPage.parent = &mainPage;
    sensorPage.itemCount = 5;
}

void MenuSystem::setupActuatorControlMenuPage()
{
    actuatorPage.title = "Actuators";
    actuatorPage.parent = &mainPage;
    if (actuatorActionsContext)
    {
        actuatorPage.items[0] = {"Pump", nullptr, [](void *ctx)
                                 { actuatorActionsContext->togglePump(); }};
        actuatorPage.items[1] = {"Light", nullptr, [](void *ctx)
                                 { actuatorActionsContext->toggleLight(); }};
        actuatorPage.items[2] = {"Fan", nullptr, [](void *ctx)
                                 { actuatorActionsContext->toggleFan(); }};
        actuatorPage.items[3] = {"(back)", nullptr, nullptr};
        actuatorPage.itemCount = 4;
    }
}

void MenuSystem::commitSelectedPlantTypeCallback(void *ctx)
{
    int plantTypeIndex = (int)(intptr_t)ctx;

    if (plantTypeIndex < 0 || plantTypeIndex >= PLANT_TYPE_COUNT)
    {
        Serial.println("ERROR: Invalid plant type index");
        return;
    }

    if (activePlantIndex < 0 || activePlantIndex >= PLANT_PAGES_COUNT)
    {
        Serial.println("ERROR: Invalid active plant index");
        return;
    }

    const char *plantName = (const char *)pgm_read_word(&(plantNames[plantTypeIndex]));

    strncpy_P(slotsPlants[activePlantIndex].label, plantName, 15);
    slotsPlants[activePlantIndex].label[15] = '\0';

    slotsPlants[activePlantIndex].isSet = true;

    plantsPage.items[activePlantIndex] = {
        slotsPlants[activePlantIndex].label,
        slotsPlants[activePlantIndex].page,
        nullptr,
        nullptr};

    slotsPlants[activePlantIndex].page->title = slotsPlants[activePlantIndex].label;

    globalMenuPtr->savePlantData();

    Serial.print("Plant ");
    Serial.print(activePlantIndex);
    Serial.print(" set to: ");
    Serial.println(slotsPlants[activePlantIndex].label);
}

void MenuSystem::setActivePlantCallback(void *ctx)
{
    activePlantIndex = (int)(intptr_t)ctx;
}

void MenuSystem::savePlantData()
{
    for (int i = 0; i < PLANT_PAGES_COUNT; i++)
    {
        EEPROM.write(EEPROM_ADDR + (i * 17), slotsPlants[i].isSet ? 1 : 0);

        for (int j = 0; j < 16; j++)
        {
            EEPROM.write(EEPROM_ADDR + (i * 17) + 1 + j, slotsPlants[i].label[j]);
        }
    }
    Serial.println(F("EEPROM written!"));
}

void MenuSystem::loadPlantData()
{
    for (int i = 0; i < PLANT_PAGES_COUNT; i++)
    {
        slotsPlants[i].isSet = EEPROM.read(EEPROM_ADDR + (i * 17)) == 1;

        if (slotsPlants[i].isSet)
        {
            for (int j = 0; j < 16; j++)
            {
                slotsPlants[i].label[j] = EEPROM.read(EEPROM_ADDR + (i * 17) + 1 + j);
            }
        }

        slotsPlants[i].page = (i == 0)   ? &plant1Page
                              : (i == 1) ? &plant2Page
                                         : &plant3Page;
    }
    Serial.println(F("EEPROM read!"));
}