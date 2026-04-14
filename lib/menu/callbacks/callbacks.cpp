#include "menuSystem.h"
#include <Arduino.h>

extern uint8_t activePlantIndex;
extern const MenuPage *selectedPlantPages[3];
extern MenuSystem *globalMenuPtr;

void MenuSystem::plantSelectionCallback(void *ctx)
{
    const MenuPage *chosenPage = (const MenuPage *)ctx;

    selectedPlantPages[activePlantIndex] = chosenPage;

    const __FlashStringHelper *title = (const __FlashStringHelper *)pgm_read_ptr(&(chosenPage->title));
    Serial.print(F("Selected "));
    Serial.println(title);

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
