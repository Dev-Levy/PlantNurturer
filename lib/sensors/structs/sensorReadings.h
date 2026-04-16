#pragma once

struct SensorReadings
{
    int16_t airHumidity;
    int16_t airTemp;
    int16_t soilTemp;
    uint8_t soilMoisture;
    uint8_t light : 1;
} __attribute__((packed));