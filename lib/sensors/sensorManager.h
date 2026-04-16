#pragma once
#include <Arduino.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "structs/sensorReadings.h"

#include "config.h"

class SensorManager
{
public:
    SensorManager();
    void begin();
    SensorReadings readAll();

private:
    DHT dht;
    OneWire oneWire;
    DallasTemperature sensors;
    SensorReadings lastReadings;

    uint32_t lastDhtRead = 0;
};
