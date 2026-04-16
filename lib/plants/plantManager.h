#pragma once
#include "structs/plantConfig.h"

class PlantManager
{
public:
    PlantManager();

    PlantConfig getPlantConfig(uint8_t index);

private:
    static const PlantConfig library[] PROGMEM;
};