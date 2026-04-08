#include "SensorManager.h"

SensorManager::SensorManager()
    : dht(DHT_PIN, DHT_TYPE),
      oneWire(SOIL_TEMP_PIN),
      sensors(&oneWire),
      lastReadings()
{
}

SensorReadings SensorManager::readAll()
{
    // float newTemp = readAirTempSensor();
    // float newHumid = readHumiditySensor();

    // if (!isnan(newTemp) && !isnan(newHumid))
    // {
    //     lastReadings.airTemp = newTemp;
    //     lastReadings.humidity = newHumid;
    // }

    // lastReadings.light = readLightSensor();
    // lastReadings.soilTemp = readSoilTempSensor();
    // lastReadings.soilMoisture = readSoilMoistureSensor();

    // Serial.print("Light: ");
    // Serial.print(lastReadings.light);
    // Serial.print(" | Moisture: ");
    // Serial.print(lastReadings.soilMoisture);
    // Serial.print(" | Humidity: ");
    // Serial.print(lastReadings.humidity);
    // Serial.print(" | Air Temp: ");
    // Serial.println(lastReadings.airTemp);
    // Serial.print(" | Soil Temp: ");
    // Serial.println(lastReadings.soilTemp);

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

    return humidity;
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

    return temp;
}

float SensorManager::readSoilTempSensor()
{
    sensors.requestTemperatures();
    return sensors.getTempCByIndex(0);
}