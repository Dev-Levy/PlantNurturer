#pragma once
#include <Arduino.h>

struct PlantConfig
{
    uint8_t idx;            // Plant idx
    uint8_t sunnyHours;     // Required light hours
    uint16_t waterLimit;    // At what soil moisture level to water (e.g., 30%)
    uint16_t waterMl;       // Watering amount
    uint16_t idealHumidity; // Watering amount
    int16_t idealTemp;      // Ideal temp (e.g., 22.5C = 225)
    int16_t idealSoilTemp;  // Ideal temp (e.g., 22.5C = 225)
};