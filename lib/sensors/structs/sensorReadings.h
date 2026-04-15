#pragma once

struct SensorReadings
{
    uint8_t soilMoisture;
    float airHumidity;
    float airTemp;
    float soilTemp;
    bool light;
} __attribute__((packed));