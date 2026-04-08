#include <avr/pgmspace.h>

#include "MenuSystem.h"

static char plantTypeLabels[PLANT_TYPE_COUNT][16];
static char plantSlotLabels[3][16] = {"Plant1", "Plant2", "Plant3"};
static char plantPageTitles[3][16] = {"Plant1", "Plant2", "Plant3"};

static IActuatorActions *actuatorActionsContext = nullptr;
static MenuSystem *globalMenuPtr = nullptr;
int activePlantIndex = 0;

MenuSystem::MenuSystem(TFTManager &tftMgr, ISensorActions &sensorActions, IActuatorActions &actuatorActions)
    : tftManager(tftMgr),
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

    setupMenuConfiguration();
    setupActuatorsPage();
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
                activeItem.callback();
            }
            currentCursor = 0;
        }
        // action
        else if (activeItem.callback != nullptr)
        {
            activeItem.callback();
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

void MenuSystem::draw()
{
    drawPage(currentPage);
}

void MenuSystem::drawPage(MenuPage *page)
{
    tftManager.clearScreen();
    auto tft = tftManager.getTFT();

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
    auto tft = tftManager.getTFT();
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
        sprintf(buffer, "Soil Moisture: %d%%", currentReadings.soilMoisture);
        break;
    default:
        sprintf(buffer, "Unknown Sensor");
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

int MenuSystem::getCursorPosition()
{
    return currentCursor;
}

static void commitSelectedPlantType()
{
    int index = globalMenuPtr->getCursorPosition();
    const char *flashName = (const char *)pgm_read_word(&(plantNames[index]));

    // plants menu items
    strcpy_P(plantSlotLabels[activePlantIndex], flashName);
    plantsPage.items[activePlantIndex].label = plantSlotLabels[activePlantIndex];
    strcpy(plantPageTitles[activePlantIndex], plantSlotLabels[activePlantIndex]);

    // plant page title
    if (activePlantIndex == 0)
    {
        plant1Page.title = plantSlotLabels[activePlantIndex];
        plantsPage.items[activePlantIndex].targetPage = &plant1Page;
    }
    else if (activePlantIndex == 1)
    {
        plant2Page.title = plantSlotLabels[activePlantIndex];
        plantsPage.items[activePlantIndex].targetPage = &plant2Page;
    }
    else if (activePlantIndex == 2)
    {
        plant3Page.title = plantSlotLabels[activePlantIndex];
        plantsPage.items[activePlantIndex].targetPage = &plant3Page;
    }
}

void MenuSystem::setupMenuConfiguration()
{
    // --- Home Page ---
    homePage.title = "Welcome!";
    homePage.items[0] = {"Enter Main Menu", &mainPage};
    homePage.itemCount = 1;
    homePage.parent = nullptr;

    // --- Main Page ---
    mainPage.title = "Main Menu";
    mainPage.parent = &homePage;
    mainPage.items[0] = {"Plants", &plantsPage};
    mainPage.items[1] = {"Sensors", &sensorPage};
    mainPage.items[2] = {"Actuators", &actuatorPage};
    mainPage.items[3] = {"(back)", &homePage};
    mainPage.itemCount = 4;

    // --- Plants Page ---
    plantsPage.title = "Plants";
    plantsPage.parent = &mainPage;
    plantsPage.items[0] = {plantSlotLabels[0], &setPlantPage, []()
                           { activePlantIndex = 0; }};
    plantsPage.items[1] = {plantSlotLabels[1], &setPlantPage, []()
                           { activePlantIndex = 1; }};
    plantsPage.items[2] = {plantSlotLabels[2], &setPlantPage, []()
                           { activePlantIndex = 2; }};
    plantsPage.items[3] = {"(back)", nullptr, nullptr};
    plantsPage.itemCount = 4;

    // --- Set Plant Page ---
    setPlantPage.title = "Select Plant";
    setPlantPage.parent = &plantsPage;
    setPlantPage.itemCount = PLANT_TYPE_COUNT;

    for (int i = 0; i < PLANT_TYPE_COUNT; i++)
    {
        const char *flashName = (const char *)pgm_read_word(&(plantNames[i]));
        strcpy_P(plantTypeLabels[i], flashName);

        setPlantPage.items[i].label = plantTypeLabels[i];
        setPlantPage.items[i].targetPage = &plantsPage;
        setPlantPage.items[i].callback = commitSelectedPlantType;
    }

    plant1Page.parent = &plantsPage;
    plant2Page.parent = &plantsPage;
    plant3Page.parent = &plantsPage;

    // --- Sensors Page ---
    sensorPage.title = "Live Data";
    sensorPage.parent = &mainPage;
    sensorPage.itemCount = 5;

    // --- Actuators Page ---
    actuatorPage.title = "Actuators";
    actuatorPage.parent = &mainPage;
}

void MenuSystem::setupActuatorsPage()
{
    if (actuatorActionsContext)
    {
        actuatorPage.items[0] = {"Pump", nullptr, []()
                                 { actuatorActionsContext->togglePump(); }};
        actuatorPage.items[1] = {"Light", nullptr, []()
                                 { actuatorActionsContext->toggleLight(); }};
        actuatorPage.items[2] = {"Fan", nullptr, []()
                                 { actuatorActionsContext->toggleFan(); }};
        actuatorPage.items[3] = {"(back)", nullptr, nullptr};
        actuatorPage.itemCount = 4;
    }
}
