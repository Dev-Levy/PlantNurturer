#pragma once
#include <Arduino.h>

#include <BH1750.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include "structs/sensorReadings.h"

#include "config.h"

class SensorManager
{
public:
    SensorManager();
    void begin();
    SensorReading readAll();

private:
    Adafruit_AHTX0 aht;
    BH1750 lightMeter;
    OneWire oneWire;
    DallasTemperature sensors;

    struct
    {
        uint8_t lightReady : 1;
        uint8_t ahtReady : 1;
    } state;

    SensorReading lastReading;
    uint32_t lastSlowRead = 0;
};
