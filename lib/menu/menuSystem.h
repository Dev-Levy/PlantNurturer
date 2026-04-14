
#pragma once

#include <config.h>

#include "pages/pages.h"
#include "navigationKeys/keys.h"

#include "displayActionsInterface.h"
#include "sensorActionsInterface.h"
#include "actuatorActionsInterface.h"

class MenuSystem
{
public:
    explicit MenuSystem(IDisplayActions &display, ISensorActions &sensorActions, IActuatorActions &actuatorActions);

    void begin();
    void draw();
    void processKey(KeyPress key);
    void updateSensorValues();

    static void plantSelectionCallback(void *ctx);
    static void togglePumpCallBack(void *ctx);
    static void toggleLightCallBack(void *ctx);
    static void toggleFanCallBack(void *ctx);

private:
    IDisplayActions &display;
    ISensorActions &sensorActions;
    IActuatorActions &actuatorActions;

    uint8_t currentCursor;
    const MenuPage *currentPage;
    SensorReadings currentReadings;

    void drawMenuItems();
    void drawItem(int y, const char *text, bool selected);
};