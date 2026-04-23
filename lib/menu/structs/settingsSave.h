#pragma once
#include <Arduino.h>
#include "config.h"
#include "plantManager.h"

struct SettingsSave
{
    int8_t plantIds[PLANT_COUNT];
    PlantConfig configs[PLANT_COUNT];
    uint8_t mainPlantIndex;
    uint32_t growthStartTime;
    uint32_t magic;
};