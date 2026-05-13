#pragma once

struct SensorReading
{
    uint16_t airTemp;
    uint16_t airHumidity;
    uint16_t soilTemp;
    uint16_t lightLux;
    uint16_t soilMoistureAnalog;
    uint8_t soilMoisture;
    uint8_t light : 1;
} __attribute__((packed));