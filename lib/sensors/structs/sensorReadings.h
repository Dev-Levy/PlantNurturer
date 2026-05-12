#pragma once

struct SensorReading
{
    int16_t airTemp;
    int16_t airHumidity;
    int16_t soilTemp;
    uint16_t lightLux;
    uint16_t soilMoistureAnalog;
    uint8_t soilMoisture;
    uint8_t light : 1;
} __attribute__((packed));