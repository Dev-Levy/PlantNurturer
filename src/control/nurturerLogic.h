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

private:
    TimeManager &clock;
    SensorManager &sensor;
    ActuatorManager &actuator;
    PlantManager &plant;

    SensorReadings data;

    void controlPump(const PlantConfig &config);
    void controlLight(const PlantConfig &config);
    void controlFan(const PlantConfig &config);
};