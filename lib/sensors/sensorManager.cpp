#include "sensorManager.h"

static constexpr int16_t DRY_VAL = 500;
static constexpr int16_t WET_VAL = 200;

SensorManager::SensorManager() : oneWire(SOIL_TEMP_PIN), soilTempMeter(&oneWire)
{
}

void SensorManager::begin()
{
    Wire.begin();

    state.lightReady = lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
    if (!state.lightReady)
    {
        DEBUG_PRINTLN(F("BH1750 not found"));
    }

    state.ahtReady = aht.begin();
    if (!state.ahtReady)
    {
        DEBUG_PRINTLN(F("AHT20 not found"));
    }

    soilTempMeter.begin();
    soilTempMeter.setWaitForConversion(false);
    soilTempMeter.requestTemperatures();

    pinMode(SOIL_MOIST_PIN, INPUT);
}

const SensorReading &SensorManager::readAll()
{
    uint32_t currentMillis = millis();

    if (currentMillis - lastSlowRead > SENSOR_READ_WAIT_TIME * 1000)
    {
        if (state.ahtReady)
        {
            sensors_event_t h, t;
            if (aht.getEvent(&h, &t))
            {
                lastReading.airTemp = (int16_t)(t.temperature * 10);
                lastReading.airHumidity = (int16_t)(h.relative_humidity * 10);
            }
            else
            {
                lastReading.airTemp = 0;
                lastReading.airHumidity = 0;
            }
        }
        else
        {
            lastReading.airTemp = 0;
            lastReading.airHumidity = 0;
        }

        float soilTempC = soilTempMeter.getTempCByIndex(0);
        if (soilTempC != DEVICE_DISCONNECTED_C)
        {
            lastReading.soilTemp = (int16_t)(soilTempC * 10);
        }

        soilTempMeter.requestTemperatures();

        lastSlowRead = currentMillis;
    }

    if (state.lightReady)
    {
        float lux = lightMeter.readLightLevel();
        if (lux >= 0)
        {
            lastReading.lightLux = (uint16_t)lux;
            lastReading.light = (lux > LUX_LIMIT) ? 1 : 0;
        }
        else
        {
            lastReading.lightLux = 0;
            lastReading.light = 0;
        }
    }
    else
    {
        lastReading.lightLux = 0;
        lastReading.light = 0;
    }

    int16_t rawMoisture = analogRead(SOIL_MOIST_PIN);
    if (rawMoisture <= 5 || rawMoisture >= 1018)
    {
        lastReading.soilMoisture = 0;
    }
    else
    {
        int32_t percentage = map(rawMoisture, DRY_VAL, WET_VAL, 0, 100);
        lastReading.soilMoisture = (uint8_t)constrain(percentage, 0, 100);
    }

    return lastReading;
}