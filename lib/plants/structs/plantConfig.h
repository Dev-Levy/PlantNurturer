#pragma once
#include <Arduino.h>

struct PlantConfig
{
    uint8_t idx;        // Plant idx
    uint8_t sunnyHours; // Required light hours
    uint16_t waterMl;   // Watering amount
    int16_t minTemp;    // Ideal temp (e.g., 22.5C = 225)
    int16_t maxTemp;    // Ideal temp (e.g., 22.5C = 225)
};