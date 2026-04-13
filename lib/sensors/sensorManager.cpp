#include "sensorManager.h"

SensorManager::SensorManager()
    : dht(DHT_PIN, DHT_TYPE),
      oneWire(SOIL_TEMP_PIN),
      sensors(&oneWire),
      lastReadings()
{
}
SensorReadings SensorManager::readAll()
{
    if (millis() - lastDhtRead > 2000)
    {
        float h = dht.readHumidity();
        float t = dht.readTemperature();

        if (!isnan(h))
        {
            lastReadings.humidity = h;
        }
        if (!isnan(t))
        {
            lastReadings.airTemp = t;
        }

        lastDhtRead = millis();
    }

    lastReadings.light = digitalRead(LDR_PIN);
    lastReadings.soilMoisture = analogRead(SOIL_MOIST_PIN);

    sensors.requestTemperatures();
    lastReadings.soilTemp = sensors.getTempCByIndex(0);

    return lastReadings;
}