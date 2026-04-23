#include "sensorManager.h"

SensorManager::SensorManager()
{
}

void SensorManager::begin()
{
    Wire.begin();

    state.lightReady = lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
    if (!state.lightReady)
    {
        Serial.println(F("BH1750 not found"));
    }

    state.ahtReady = aht.begin();
    if (!state.ahtReady)
    {
        Serial.println(F("AHT20 not found"));
    }

    pinMode(SOIL_MOIST_PIN, INPUT);
}

SensorReadings SensorManager::readAll()
{
    uint32_t currentMillis = millis();

    if (currentMillis - lastSlowRead > 2000)
    {
        if (state.ahtReady)
        {
            sensors_event_t humidity, temp;
            aht.getEvent(&humidity, &temp);

            if (!isnan(temp.temperature))
                lastReadings.airTemp = (int16_t)(temp.temperature * 10);
            else
                lastReadings.airTemp = 0;

            if (!isnan(humidity.relative_humidity))
                lastReadings.airHumidity = (int16_t)(humidity.relative_humidity * 10);
            else
                lastReadings.airHumidity = 0;
        }
        else
        {
            lastReadings.airTemp = 0;
            lastReadings.airHumidity = 0;
        }

        lastSlowRead = currentMillis;
    }

    if (state.lightReady)
    {
        float lux = lightMeter.readLightLevel();
        if (!isnan(lux) && lux >= 0)
        {
            lastReadings.lightLux = (uint16_t)lux;
            lastReadings.light = (lux > 50) ? 1 : 0;
        }
        else
        {
            lastReadings.lightLux = 0;
            lastReadings.light = 0;
        }
    }
    else
    {
        lastReadings.lightLux = 0;
        lastReadings.light = 0;
    }

    int16_t rawMoisture = analogRead(SOIL_MOIST_PIN);
    if (rawMoisture <= 1 || rawMoisture >= 1022)
    {
        lastReadings.soilMoisture = 0;
    }
    else
    {
        int32_t percentage = ((int32_t)(rawMoisture - 1023) * 100) / (200 - 1023);
        lastReadings.soilMoisture = (uint8_t)constrain(percentage, 0, 100);
    }

    return lastReadings;
}