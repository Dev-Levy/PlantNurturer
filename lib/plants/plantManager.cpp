#include "plantManager.h"

static const WateringLookupEntry wateringLookup[] PROGMEM = {
    {5, 1},
    {10, 2},
    {15, 3},
    {20, 4},
    {25, 5},
    {30, 6},
    {40, 8},
    {50, 10},
};

// id, sunny hours, water limit, water ml, min temp (*10), max temp (*10), min humi (*10), max humi (*10)
static const PlantConfig library[] PROGMEM = {
    {0, 12, 50, 10, 200, 300, 100, 500}, // tomato ID: 0
    {1, 14, 30, 15, 180, 260, 100, 500}, // chili  ID: 1
    {2, 6, 20, 8, 190, 240, 100, 500},   // mint   ID: 2
    {3, 8, 25, 10, 180, 250, 100, 500},  // basil  ID: 3
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