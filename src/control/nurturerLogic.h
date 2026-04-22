#pragma once

#include "timeManager.h"
#include "sensorManager.h"
#include "actuatorManager.h"
#include "plantManager.h"

class NurturerLogic
{
public:
    NurturerLogic(TimeManager &clock, SensorManager &sensor, ActuatorManager &actuator, PlantManager &plant);

    void control(uint8_t plantIndex);

private:
    TimeManager &clock;
    SensorManager &sensor;
    ActuatorManager &actuator;
    PlantManager &plant;

    SensorReadings data;
    PlantConfig currentPlantConfig;
};