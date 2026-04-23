#include "nurturerLogic.h"

static unsigned long lastSensorRefresh = 0;
static unsigned long lastConfigIndex = 0;

static unsigned long pumpStart = 0;

static unsigned long pumpWaitTime = 0;
static unsigned long lightWaitTime = 0;
static unsigned long fanWaitTime = 0;

static bool pumpRunning = false;
static bool lightOn = false;
static bool fanRunning = false;

NurturerLogic::NurturerLogic(TimeManager &clock, SensorManager &sensor, ActuatorManager &actuator, PlantManager &plant)
    : clock(clock), sensor(sensor), actuator(actuator), plant(plant)
{
}

void NurturerLogic::control(const PlantConfig &config)
{
    if (config.idx != lastConfigIndex)
    {
        lastConfigIndex = config.idx;

        Serial.print(F("Sunny hours: "));
        Serial.print(config.sunnyHours);
        Serial.println(F(" h"));

        Serial.print(F("Light limit: "));
        Serial.print(LUX_LIMIT);
        Serial.println(F(" lux"));

        Serial.print(F("Water limit: "));
        Serial.print(config.waterLimit);
        Serial.println(F(" %"));

        Serial.print(F("Water ml: "));
        Serial.println(config.waterMl);

        Serial.print(F("Min temp: "));
        Serial.println(config.minTemp);
        Serial.print(F("Max temp: "));
        Serial.println(config.maxTemp);
        Serial.print(F("Min humidity: "));
        Serial.println(config.minHumi);
        Serial.print(F("Max humidity: "));
        Serial.println(config.maxHumi);

        Serial.println(F("--------------------"));
    }

    if (millis() - lastSensorRefresh > 2000)
    {
        data = sensor.readAll();
        lastSensorRefresh = millis();
    }

    controlPump(config);
    controlLight(config);
    controlFan(config);
}

void NurturerLogic::controlPump(const PlantConfig &config)
{
    const uint8_t wateringSeconds = plant.getWateringSeconds(config.waterMl);
    const unsigned long wateringDurationMs = (unsigned long)wateringSeconds * 1000UL;

    if (!pumpRunning && data.soilMoisture < config.waterLimit &&
        (millis() - pumpWaitTime >= PUMP_COOLDOWN_IN_SECONDS * 1000UL))
    {
        Serial.println(F("Turning on pump"));

        actuator.togglePump();
        pumpRunning = true;
        pumpStart = millis();
    }

    if (pumpRunning &&
        (millis() - pumpStart >= wateringDurationMs))
    {
        Serial.println(F("Turning off pump"));

        actuator.togglePump();
        pumpRunning = false;
        pumpWaitTime = millis();
    }
}

void NurturerLogic::controlLight(const PlantConfig &config)
{
    if (!lightOn && !data.light &&
        (millis() - lightWaitTime >= LIGHT_COOLDOWN_IN_SECONDS * 1000UL))
    {
        Serial.println(F("Turning on light"));

        actuator.toggleLight();
        lightOn = true;
    }

    if (lightOn && data.light)
    {
        Serial.println(F("Turning off light"));

        actuator.toggleLight();
        lightWaitTime = millis();
        lightOn = false;
    }
}

void NurturerLogic::controlFan(const PlantConfig &config)
{
    if (!fanRunning && data.airTemp > (float)config.maxTemp / 10 &&
        (millis() - fanWaitTime >= FAN_COOLDOWN_IN_SECONDS * 1000UL))
    {
        Serial.println(F("Turning on fan"));

        actuator.toggleFan();
        fanRunning = true;
    }

    if (fanRunning && data.airTemp < (float)config.minTemp / 10)
    {
        Serial.println(F("Turning off fan"));

        actuator.toggleFan();
        fanWaitTime = millis();
        fanRunning = false;
    }
}
