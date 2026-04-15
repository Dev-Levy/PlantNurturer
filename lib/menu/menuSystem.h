
#pragma once

#include <config.h>

#include "pages/pages.h"
#include "navigationKeys/keys.h"

#include "TimeActionsInterface.h"
#include "displayActionsInterface.h"
#include "sensorActionsInterface.h"
#include "actuatorActionsInterface.h"

class MenuSystem
{
public:
    explicit MenuSystem(ITimeActions &time, IDisplayActions &display, ISensorActions &sensorActions, IActuatorActions &actuatorActions);

    void begin();
    void draw();
    void processKey(KeyPress key);
    void updateSensorValues();

    static void plantSelectionCallback(void *ctx);
    static void plantSetAsMainCallback(void *ctx);
    static void plantRemoveCallback(void *ctx);
    static void togglePumpCallBack(void *ctx);
    static void toggleLightCallBack(void *ctx);
    static void toggleFanCallBack(void *ctx);

private:
    ITimeActions &time;
    IDisplayActions &display;
    ISensorActions &sensorActions;
    IActuatorActions &actuatorActions;

    static MenuSystem *globalMenuPtr;
    static uint8_t currentCursor;
    static const MenuPage *currentPage;

    static uint8_t mainPlantIndex;
    static uint8_t activePlantIndex;
    static const MenuPage *selectedPlantPages[3];

    SensorReadings currentReadings;

    void drawTimeRow();
    void drawHomePageMenuItems();
    // void drawMainPageMenuItems();
    void drawPlantsPageMenuItems();
    void drawSensorPageMenuItems();
    // void drawActuatorPageMenuItems();
    void drawMenuItems();
    void drawItem(int y, const __FlashStringHelper *text, bool selected);
    void drawItem(int y, const char *text, bool selected);
    void getSensorString(uint8_t index, char *buffer);
    const char *getMonthName(uint8_t month);
};