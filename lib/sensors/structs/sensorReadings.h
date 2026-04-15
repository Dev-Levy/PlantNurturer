#pragma once

struct SensorReadings
{
    uint16_t soilMoisture;
    float airHumidity;
    float airTemp;
    float soilTemp;
    bool light;
} __attribute__((packed));