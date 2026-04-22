
#pragma once

#include <config.h>

#include "pages/pages.h"
#include "navigationKeys/keys.h"
#include "helpers/helper.h"

#include "timeManager.h"
#include "displayManager.h"
#include "sensorManager.h"
#include "actuatorManager.h"
#include "plantManager.h"

class MenuSystem;
extern MenuSystem *globalMenuPtr;
class MenuSystem
{
public:
    explicit MenuSystem(TimeManager &time, DisplayManager &display, SensorManager &sensor, ActuatorManager &actuator, PlantManager &plant);

    uint8_t plantConfigIndex = 0;

    void begin();
    void draw();
    void processKey(KeyPress key);
    void refresh();

    static void plantSelectionCallback(void *ctx);
    static void plantSetAsMainCallback(void *ctx);
    static void plantRemoveCallback(void *ctx);
    static void togglePumpCallBack(void *ctx);
    static void toggleLightCallBack(void *ctx);
    static void toggleFanCallBack(void *ctx);

private:
    TimeManager &time;
    DisplayManager &display;
    SensorManager &sensor;
    ActuatorManager &actuator;
    PlantManager &plant;

    uint8_t currentCursor = 0;
    const MenuPage *currentPage = nullptr;
    SensorReadings currentReading;

    uint8_t mainPlantIndex = 0;
    uint8_t activePlantIndex = 0;
    const MenuPage *selectedPlantPages[PLANT_COUNT] = {nullptr};

    void drawTimeRow();
    void drawGrowingRow();
    void drawHomePageMenuItems();
    // void drawMainPageMenuItems();
    void drawPlantsPageMenuItems();
    void drawSensorPageMenuItems();
    // void drawActuatorPageMenuItems();
    void drawMenuItems();

    void drawSensorPageMenuItem(uint8_t index, uint8_t y, bool isSelected, const SensorReadings &data, const __FlashStringHelper *label);
    void setItemDrawingProps(bool isSelected, uint8_t y);
    void print2Digits(uint8_t value);
};