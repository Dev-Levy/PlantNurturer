#include "MenuSystem.h"

static IActuatorActions *actuatorActionsContext = nullptr;

MenuSystem::MenuSystem(TFTManager &tftMgr, ISensorActions &sensorActions, IActuatorActions &actuatorActions)
    : tftManager(tftMgr),
      sensorActions(sensorActions),
      actuatorActions(actuatorActions),
      selected(0),
      currentPage(nullptr),
      currentReadings()
{
}

void MenuSystem::begin()
{
    actuatorActionsContext = &actuatorActions;
    currentPage = &homePage;

    setupMenuConfiguration();
    setutActuatorsPage();
    draw();
}

void MenuSystem::processKey(int key)
{
    switch (key)
    {
    case 1: // UP
        selected = (selected == 0) ? currentPage->itemCount - 1 : selected - 1;
        break;

    case 2: // DOWN
        selected = (selected == currentPage->itemCount - 1) ? 0 : selected + 1;
        break;

    case 3:
    {
        MenuItem &activeItem = currentPage->items[selected];

        // nav
        if (activeItem.targetPage != nullptr)
        {
            currentPage = activeItem.targetPage;
            selected = 0;
            if (activeItem.callback != nullptr)
            {
                activeItem.callback();
            }
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
            selected = 0;
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

        bool isSelected = (i == selected);

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
        bool isSelected = (i == selected);

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

void MenuSystem::setutActuatorsPage()
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