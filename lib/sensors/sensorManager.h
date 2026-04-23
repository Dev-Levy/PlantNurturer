#pragma once
#include <Arduino.h>

#include <BH1750.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>

// #include <OneWire.h>
// #include <DallasTemperature.h>

#include "structs/sensorReadings.h"

#include "config.h"

class SensorManager
{
public:
    SensorManager();
    void begin();
    SensorReadings readAll();

private:
    Adafruit_AHTX0 aht;
    BH1750 lightMeter;

    struct
    {
        uint8_t lightReady : 1;
        uint8_t ahtReady : 1;
        uint8_t bmpReady : 1;
    } state;

    // OneWire oneWire;
    // DallasTemperature sensors;

    SensorReadings lastReadings;
    uint32_t lastSlowRead = 0;
};
