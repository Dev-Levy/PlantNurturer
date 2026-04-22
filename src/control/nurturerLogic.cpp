#include "nurturerLogic.h"

static unsigned long lastSensorRefresh = 0;

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

    if (!pumpRunning &&
        data.soilMoisture < config.waterLimit &&
        millis() - pumpWaitTime < PUMP_COOLDOWN_IN_SECONDS * 1000UL)
    {
        actuator.togglePump();
        pumpRunning = true;
        pumpStart = millis();
    }

    if (pumpRunning &&
        (millis() - pumpStart >= wateringDurationMs))
    {
        actuator.togglePump();
        pumpRunning = false;
        pumpWaitTime = millis();
    }
}

void NurturerLogic::controlLight(const PlantConfig &config)
{
    if (!lightOn &&
        !data.light &&
        millis() - lightWaitTime < LIGHT_COOLDOWN_IN_SECONDS * 1000UL)
    {
        actuator.toggleLight();
        lightOn = true;
    }

    if (lightOn &&
        data.light)
    {
        actuator.toggleLight();
        lightWaitTime = millis();
        lightOn = false;
    }
}

void NurturerLogic::controlFan(const PlantConfig &config)
{
    if (!fanRunning &&
        data.airTemp > (float)config.maxTemp / 10 &&
        millis() - fanWaitTime < FAN_COOLDOWN_IN_SECONDS * 1000UL)
    {
        actuator.toggleFan();
        fanRunning = true;
    }

    if (fanRunning &&
        data.airTemp < (float)config.minTemp / 10)
    {
        actuator.toggleFan();
        fanWaitTime = millis();
        fanRunning = false;
    }
}
