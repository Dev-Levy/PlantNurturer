
#pragma once
#include "../tft/TFTManager.h"

#include "config.h"
#include "menuConfig.h"
#include "helpers/menuPage.h"
#include "sensors/sensorActionsInterface.h"
#include "actuators/actuatorActionsInterface.h"

class MenuSystem
{
public:
    explicit MenuSystem(TFTManager &tftManager, ISensorActions &sensorActions, IActuatorActions &actuatorActions);

    void begin();
    void draw();
    void processKey(int key);
    void updateSensorValues();

private:
    TFTManager &tftManager;
    ISensorActions &sensorActions;
    IActuatorActions &actuatorActions;

    int selected;
    MenuPage *currentPage;
    SensorReadings currentReadings;

    void drawPage(MenuPage *page);
    void drawDynamicSensorData();
    void drawItem(int y, const char *text, bool selected);
    void getSensorString(int index, char *buffer);

    void setutActuatorsPage();
};