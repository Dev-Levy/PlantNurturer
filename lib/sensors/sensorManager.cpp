#include "sensorManager.h"

SensorManager::SensorManager()
// : oneWire(SOIL_TEMP_PIN),
//   sensors(&oneWire)
{
}
void SensorManager::begin()
{
    Wire.begin();
    if (!lightMeter.begin(BH1750::CONTINUOUS_HIGH_RES_MODE))
    {
        Serial.println(F("Error initializing BH1750"));
    }
    if (!aht.begin())
    {
        Serial.println(F("Error initializing AHT20!"));
    }
    if (!bmp.begin(0x76))
    {
        Serial.println(F("Error initializing BMP280!"));
    }

    // sensors.begin();
    // pinMode(LDR_PIN, INPUT);

    pinMode(SOIL_MOIST_PIN, INPUT);
}

SensorReadings SensorManager::readAll()
{
    uint32_t currentMillis = millis();

    if (currentMillis - lastSlowRead > 2000)
    {

        sensors_event_t humidity, temp;
        aht.getEvent(&humidity, &temp);

        lastReadings.airTemp = (int16_t)(temp.temperature * 10);
        lastReadings.airHumidity = (int16_t)(humidity.relative_humidity * 10);
        lastReadings.pressure = (uint16_t)bmp.readPressure() / 100;

        // float humi = dht.readHumidity();
        // float airTemp = dht.readTemperature();

        // if (humi == humi)
        // {
        //     lastReadings.airHumidity = (int16_t)(humi * 10);
        // }
        // if (airTemp == airTemp)
        // {
        //     lastReadings.airTemp = (int16_t)(airTemp * 10);
        // }

        // sensors.requestTemperatures();
        // float soilTemp = sensors.getTempCByIndex(0);
        // if (soilTemp > -50)
        // {
        //     lastReadings.soilTemp = (int16_t)(soilTemp * 10);
        // }

        lastSlowRead = currentMillis;
    }

    float lux = lightMeter.readLightLevel();
    lastReadings.lightLux = (uint16_t)lux;
    lastReadings.light = (lux > 50) ? 1 : 0;

    int16_t rawMoisture = analogRead(SOIL_MOIST_PIN);
    int32_t percentage = ((int32_t)(rawMoisture - 1023) * 100) / (200 - 1023);
    lastReadings.soilMoisture = (uint8_t)constrain(percentage, 0, 100);

    return lastReadings;
}