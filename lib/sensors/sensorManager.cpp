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
    lastReadings.airTemp = readAirTempSensor();
    lastReadings.humidity = readHumiditySensor();
    lastReadings.light = readLightSensor();
    lastReadings.soilTemp = readSoilTempSensor();
    lastReadings.soilMoisture = readSoilMoistureSensor();

    return lastReadings;
}

int SensorManager::readLightSensor()
{
    return digitalRead(LDR_PIN);
}

int SensorManager::readSoilMoistureSensor()
{
    return analogRead(SOIL_MOIST_PIN);
}

float SensorManager::readHumiditySensor()
{
    float humidity = NAN;
    static unsigned long lastRead = 0;
    if (millis() - lastRead > 2000)
    {
        humidity = dht.readHumidity();
        lastRead = millis();
    }

    return isnan(humidity) ? lastReadings.humidity : humidity;
}

float SensorManager::readAirTempSensor()
{
    float temp = NAN;
    static unsigned long lastRead = 0;
    if (millis() - lastRead > 2000)
    {
        temp = dht.readTemperature(false);
        lastRead = millis();
    }

    return isnan(temp) ? lastReadings.airTemp : temp;
}

float SensorManager::readSoilTempSensor()
{
    sensors.requestTemperatures();
    return sensors.getTempCByIndex(0);
}