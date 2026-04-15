#pragma once
#include "structs/sensorReadings.h"

class ISensorActions
{
public:
    virtual ~ISensorActions() = default;
    virtual void begin();
    virtual SensorReadings readAll() = 0;
};