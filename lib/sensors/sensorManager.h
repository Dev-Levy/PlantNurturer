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
    const SensorReading &readAll();
    TEST_CONTENT(void setMockReading(const SensorReading &mock) { lastReading = mock; state.mock = 1; })
    TEST_CONTENT(void resetMock() { state.mock = 0; })

private:
    Adafruit_AHTX0 aht;
    BH1750 lightMeter;
    OneWire oneWire;
    DallasTemperature soilTempMeter;

    struct
    {
        uint8_t lightReady : 1;
        uint8_t ahtReady : 1;
        TEST_CONTENT(uint8_t mock : 1;)
    } state;

    SensorReading lastReading;
    uint32_t lastSlowRead = 0;
};
