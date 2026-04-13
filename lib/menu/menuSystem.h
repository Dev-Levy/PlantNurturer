
#pragma once

#include <config.h>

#include "pages/pages.h"
#include "structs/plantSlot.h"
#include "displayActionsInterface.h"
#include "sensorActionsInterface.h"
#include "actuatorActionsInterface.h"

class MenuSystem
{
public:
    explicit MenuSystem(IDisplayActions &display, ISensorActions &sensorActions, IActuatorActions &actuatorActions);

    void begin();
    void draw();
    void processKey(int key);
    void updateSensorValues();

private:
    IDisplayActions &display;
    ISensorActions &sensorActions;
    IActuatorActions &actuatorActions;

    int currentCursor;
    MenuPage *currentPage;
    SensorReadings currentReadings;

    void drawPage(MenuPage *page);
    void drawDynamicSensorData();
    void drawItem(int y, const char *text, bool selected);
    void getSensorString(int index, char *buffer);

    void setupMenuPages();
    void setupHomeMenuPage();
    void setupMainMenuPage();
    void setupPlantsMenuPage();
    void setupPlantMenuPage();
    void setupSelectPlantMenuPage();
    void setupSensorDataMenuPage();
    void setupActuatorControlMenuPage();

    static void commitSelectedPlantTypeCallback(void *ctx);
    static void setActivePlantCallback(void *ctx);

    void savePlantData();
    void loadPlantData();
};