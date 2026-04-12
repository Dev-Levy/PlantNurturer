#pragma once
#include "structs/sensorReadings.h"

class ISensorActions
{
public:
    virtual ~ISensorActions() = default;
    virtual SensorReadings readAll() = 0;
};