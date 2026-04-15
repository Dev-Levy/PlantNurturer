#include "menuSystem.h"
#include <Arduino.h>

void MenuSystem::plantSelectionCallback(void *ctx)
{
    selectedPlantPages[activePlantIndex] = (const MenuPage *)ctx;

    if (globalMenuPtr)
    {
        globalMenuPtr->currentPage = &plantsPage;
        globalMenuPtr->currentCursor = 0;
    }
}

void MenuSystem::plantSetAsMainCallback(void *ctx)
{
    if (globalMenuPtr)
    {
        globalMenuPtr->mainPlantIndex = activePlantIndex;

        globalMenuPtr->currentPage = &homePage;
        globalMenuPtr->currentCursor = 0;
    }
}

void MenuSystem::plantRemoveCallback(void *ctx)
{
    selectedPlantPages[activePlantIndex] = nullptr;

    if (globalMenuPtr)
    {
        globalMenuPtr->currentPage = &plantsPage;
        globalMenuPtr->currentCursor = 0;
    }
}

void MenuSystem::togglePumpCallBack(void *ctx)
{
    if (ctx)
    {
        IActuatorActions *actuators = (IActuatorActions *)ctx;
        actuators->togglePump();
    }
}

void MenuSystem::toggleLightCallBack(void *ctx)
{
    if (ctx)
    {
        IActuatorActions *actuators = (IActuatorActions *)ctx;
        actuators->toggleLight();
    }
}

void MenuSystem::toggleFanCallBack(void *ctx)
{
    if (ctx)
    {
        IActuatorActions *actuators = (IActuatorActions *)ctx;
        actuators->toggleFan();
    }
}
