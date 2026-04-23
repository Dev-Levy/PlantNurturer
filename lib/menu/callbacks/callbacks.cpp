#include "menuSystem.h"
#include <Arduino.h>

void MenuSystem::plantSelectionCallback(void *ctx)
{
    if (!globalMenuPtr)
        return;

    globalMenuPtr->selectedPlantPages[globalMenuPtr->activePlantIndex] = (const MenuPage *)ctx;
    globalMenuPtr->currentPage = &plantsPage;
    globalMenuPtr->currentCursor = 0;
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
}

void MenuSystem::plantRemoveCallback(void *ctx)
{
    if (!globalMenuPtr)
        return;

    globalMenuPtr->selectedPlantPages[globalMenuPtr->activePlantIndex] = nullptr;
    globalMenuPtr->currentPage = &plantsPage;
    globalMenuPtr->currentCursor = 0;
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
