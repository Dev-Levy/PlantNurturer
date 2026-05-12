#pragma once

struct SensorReading
{
    int16_t airTemp;
    int16_t airHumidity;
    int16_t soilTemp;
    uint16_t soilMoisture;
    uint16_t lightLux;
    uint8_t light : 1;
} __attribute__((packed));