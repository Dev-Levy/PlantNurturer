#include "plantManager.h"

static const WateringLookupEntry wateringLookup[] PROGMEM = {
    {5, 100},
    {10, 200},
    {15, 300},
    {20, 400},
    {25, 500},
    {30, 600},
    {40, 800},
    {50, 1000},
    {75, 1500},
    {100, 2000},
};

// id, sunny hours, water limit, water ml, ideal humi (*10), ideal temp (*10), ideal soil temp (*10)
static const PlantConfig library[] PROGMEM = {
    {0, 12, 50, 10, 500, 200, 300}, // tomato ID: 0
    {1, 14, 30, 15, 400, 180, 260}, // chili  ID: 1
    {2, 6, 20, 8, 300, 190, 240},   // mint   ID: 2
    {3, 8, 25, 10, 200, 180, 250},  // basil  ID: 3
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

uint8_t PlantManager::getWateringMs(uint16_t waterMl) const
{
    WateringLookupEntry entry;
    const uint8_t count = sizeof(wateringLookup) / sizeof(entry);

    for (uint8_t i = 0; i < count; ++i)
    {
        memcpy_P(&entry, &wateringLookup[i], sizeof(entry));

        if (waterMl <= entry.waterMl)
        {
            return entry.milliseconds;
        }
    }

    memcpy_P(&entry, &wateringLookup[count - 1], sizeof(entry));
    return entry.milliseconds;
}