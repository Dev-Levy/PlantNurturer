#pragma once
#include "structs/plantConfig.h"
#include "structs/wateringLookupEntry.h"

class PlantManager
{
public:
    PlantManager();

    bool getPlantConfig(uint8_t index, PlantConfig &out) const;
    uint8_t getWateringSeconds(uint16_t waterMl) const;

private:
};