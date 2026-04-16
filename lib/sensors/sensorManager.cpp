#include "sensorManager.h"

SensorManager::SensorManager()
    : dht(DHT_PIN, DHT_TYPE),
      oneWire(SOIL_TEMP_PIN),
      sensors(&oneWire)
{
}
void SensorManager::begin()
{
    dht.begin();
    sensors.begin();

    pinMode(LDR_PIN, INPUT);
    pinMode(SOIL_MOIST_PIN, INPUT);
}

SensorReadings SensorManager::readAll()
{
    uint32_t currentMillis = millis();

    if (currentMillis - lastDhtRead > 2000)
    {
        float humi = dht.readHumidity();
        float airTemp = dht.readTemperature();
        sensors.requestTemperatures();

        if (humi == humi)
        {
            lastReadings.airHumidity = (int16_t)(humi * 10);
        }
        if (airTemp == airTemp)
        {
            lastReadings.airTemp = (int16_t)(airTemp * 10);
        }

        float soilTemp = sensors.getTempCByIndex(0);
        if (soilTemp > -50)
        {
            lastReadings.soilTemp = (int16_t)(soilTemp * 10);
        }

        lastDhtRead = currentMillis;
    }

    lastReadings.light = !digitalRead(LDR_PIN) ? 1 : 0;

    int16_t rawMoisture = analogRead(SOIL_MOIST_PIN);
    int32_t percentage = ((int32_t)(rawMoisture - 1023) * 100) / (200 - 1023);
    lastReadings.soilMoisture = (uint8_t)constrain(percentage, 0, 100);

    return lastReadings;
}