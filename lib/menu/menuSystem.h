
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