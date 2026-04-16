#include "plantManager.h"

const PlantConfig PlantManager::library[] PROGMEM = {
    {0, 12, 500, 10, 30},
    {1, 14, 300, 15, 40},
    {2, 6, 200, 8, 30},
    {3, 8, 250, 10, 35},
};

PlantManager::PlantManager()
{
}

PlantConfig PlantManager::getPlantConfig(uint8_t index)
{
    PlantConfig config;
    memcpy_P(&config, &library[index], sizeof(PlantConfig));
    return config;
}