#pragma once

struct SensorReadings
{
    uint16_t soilMoisture; // 0-1023
    float humidity;
    float airTemp;
    float soilTemp;
    bool light;            // Changed from int since you use digitalRead
} __attribute__((packed)); // Forces the compiler to not add empty padding bytes