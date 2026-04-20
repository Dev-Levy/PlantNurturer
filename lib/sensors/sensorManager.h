#pragma once
#include <Arduino.h>

#include <BH1750.h>
#include <Wire.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>

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
    Adafruit_BMP280 bmp;
    BH1750 lightMeter;

    // OneWire oneWire;
    // DallasTemperature sensors;

    SensorReadings lastReadings;
    uint32_t lastDhtRead = 0;
};
