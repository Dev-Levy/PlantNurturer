#pragma once
#include "structs/plantConfig.h"
#include "structs/wateringLookupEntry.h"

class PlantManager
{
public:
    PlantManager();

    PlantConfig getPlantConfig(uint8_t index);
    uint8_t getWateringSeconds(uint16_t waterMl) const;

private:
};