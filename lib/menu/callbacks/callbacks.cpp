#include "menuSystem.h"
#include "pages/pages.h"
#include <Arduino.h>

void MenuSystem::plantSelectionCallback(void *ctx)
{
    if (!globalMenuPtr || !ctx)
        return;

    const MenuPage *page = (const MenuPage *)ctx;
    uint8_t index = globalMenuPtr->activePlantIndex;

    globalMenuPtr->selectedPlantPages[index] = page;

    uint8_t configId = 0;
    if (page == &tomatoPage)
        configId = TOMATO_CONFIG_ID;
    else if (page == &chiliPage)
        configId = CHILI_CONFIG_ID;
    else if (page == &mintPage)
        configId = MINT_CONFIG_ID;
    else if (page == &basilPage)
        configId = BASIL_CONFIG_ID;

    globalMenuPtr->plant.getPlantConfig(configId, globalMenuPtr->storedConfigs[index]);

    globalMenuPtr->currentPage = &plantsPage;
    globalMenuPtr->currentCursor = 0;

    globalMenuPtr->saveSettings();
}

void MenuSystem::plantStartGrowingCallback(void *ctx)
{
    if (!globalMenuPtr)
        return;

    if (!ctx)
        return;
    ((TimeManager *)ctx)->resetWeekCounter(((TimeManager *)ctx)->getUnixNow());

    globalMenuPtr->mainPlantIndex = globalMenuPtr->activePlantIndex;
    globalMenuPtr->currentPage = &homePage;
    globalMenuPtr->currentCursor = 0;

    globalMenuPtr->saveSettings();
}

void MenuSystem::plantRemoveCallback(void *ctx)
{
    if (!globalMenuPtr)
        return;

    globalMenuPtr->selectedPlantPages[globalMenuPtr->activePlantIndex] = nullptr;
    globalMenuPtr->currentPage = &plantsPage;
    globalMenuPtr->currentCursor = 0;

    globalMenuPtr->saveSettings();
}

void MenuSystem::togglePumpCallBack(void *ctx)
{
    if (!ctx)
        return;
    ((ActuatorManager *)ctx)->togglePump();
}

void MenuSystem::toggleLightCallBack(void *ctx)
{
    if (!ctx)
        return;
    ((ActuatorManager *)ctx)->toggleLight();
}

void MenuSystem::toggleFanCallBack(void *ctx)
{
    if (!ctx)
        return;
    ((ActuatorManager *)ctx)->toggleFan();
}
