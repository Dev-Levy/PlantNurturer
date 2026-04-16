#pragma once

#include "sensorManager.h"

class NurturerLogic
{
public:
    NurturerLogic(SensorManager sensor);

private:
    SensorManager &sensor;
    SensorReadings data;
};