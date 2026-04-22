#include "plantManager.h"

static const WateringLookupEntry wateringLookup[] PROGMEM = {
    {50, 1},
    {100, 2},
    {150, 3},
    {200, 4},
    {250, 5},
    {300, 6},
    {400, 8},
    {500, 10},
};

static const PlantConfig library[] PROGMEM = {
    {0, 12, 50, 10, 200, 300},
    {1, 14, 30, 15, 180, 260},
    {2, 6, 20, 8, 190, 240},
    {3, 8, 25, 10, 180, 250},
};

PlantManager::PlantManager()
{
}

bool PlantManager::getPlantConfig(uint8_t index, PlantConfig &out) const
{
    const uint8_t count = sizeof(library) / sizeof(library[0]);
    if (index >= count)
    {
        return false;
    }

    memcpy_P(&out, &library[index], sizeof(PlantConfig));
    return true;
}

uint8_t PlantManager::getWateringSeconds(uint16_t waterMl) const
{
    WateringLookupEntry entry;
    const uint8_t count = sizeof(wateringLookup) / sizeof(entry);

    for (uint8_t i = 0; i < count; ++i)
    {
        memcpy_P(&entry, &wateringLookup[i], sizeof(entry));

        if (waterMl <= entry.waterMl)
        {
            return entry.seconds;
        }
    }

    memcpy_P(&entry, &wateringLookup[count - 1], sizeof(entry));
    return entry.seconds;
}