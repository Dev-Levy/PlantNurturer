#pragma once
#include <Arduino.h>
#include <DHT.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include "sensorActionsInterface.h"
#include "sensorReadings.h"

#include "../config.h"

class SensorManager : public ISensorActions
{
public:
    SensorManager();

    SensorReadings readAll() override;

private:
    DHT dht;
    OneWire oneWire;
    DallasTemperature sensors;
    SensorReadings lastReadings;

    int readLightSensor();
    int readSoilMoistureSensor();
    float readHumiditySensor();
    float readAirTempSensor();
    float readSoilTempSensor();
};
