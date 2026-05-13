#pragma once

#include "timeManager.h"
#include "sensorManager.h"
#include "actuatorManager.h"
#include "plantManager.h"

class NurturerLogic
{
public:
    NurturerLogic(TimeManager &clock, SensorManager &sensor, ActuatorManager &actuator, PlantManager &plant);

    void control(const PlantConfig &config);
    void turnOffAll();

    TEST_CONTENT(void reset() {
        lastSensorRefresh = 0;
        lastConfigIndex = 99;
    })

private:
    TimeManager &clock;
    SensorManager &sensor;
    ActuatorManager &actuator;
    PlantManager &plant;

    const SensorReading &data;

    unsigned long lastSensorRefresh = 0;
    unsigned long lastConfigIndex = 99;

    TEST_CONTENT(public:)
    void controlPump(const PlantConfig &config);
    void controlLight(const PlantConfig &config);
    void controlFan(const PlantConfig &config);
    void controlPad(const PlantConfig &config);

    bool isSunnyHour(const PlantConfig &config);
};