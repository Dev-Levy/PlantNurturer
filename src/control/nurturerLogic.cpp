#include "nurturerLogic.h"

static unsigned long lastSensorRefresh = 0;
static unsigned long lastConfigIndex = 99;

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

        Serial.print(F("Ideal temp: "));
        Serial.println(config.idealTemp);
        Serial.print(F("Ideal soil temp: "));
        Serial.println(config.idealSoilTemp);

        Serial.println(F("--------------------"));
    }

    if (millis() - lastSensorRefresh > 2000)
    {
        data = sensor.readAll();
        lastSensorRefresh = millis();

        int currentHour = clock.getHour();
        int half = config.sunnyHours / 2;
        bool afternoon = currentHour > 12;
        Serial.print(F("Current hour: "));
        Serial.println(currentHour);
        Serial.print(F("Is sunny hour: "));
        Serial.println(afternoon ? currentHour - 12 <= half : 12 - currentHour <= half);
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
        actuator.turnOnPump();
        pumpRunning = true;
        pumpStart = millis();
    }

    if (pumpRunning &&
        (millis() - pumpStart >= wateringDurationMs))
    {
        actuator.turnOffPump();
        pumpRunning = false;
        pumpWaitTime = millis();
    }
}

void NurturerLogic::controlLight(const PlantConfig &config)
{
    if (!lightOn && !data.light && isSunnyHour(config) &&
        (millis() - lightWaitTime >= LIGHT_COOLDOWN_IN_SECONDS * 1000UL))
    {
        actuator.turnOnLight();
        lightOn = true;
    }

    if (lightOn && data.light)
    {
        actuator.turnOffLight();
        lightWaitTime = millis();
        lightOn = false;
    }
}

void NurturerLogic::controlFan(const PlantConfig &config)
{
    if (!fanRunning && (float)data.airTemp / 10 > (float)config.idealTemp + 5 / 10 &&
        (millis() - fanWaitTime >= FAN_COOLDOWN_IN_SECONDS * 1000UL))
    {
        actuator.turnOnFan();
        fanRunning = true;
    }

    if (fanRunning && (float)data.airTemp / 10 < (float)config.idealTemp - 5 / 10)
    {
        actuator.turnOffFan();
        fanWaitTime = millis();
        fanRunning = false;
    }
}

void NurturerLogic::controlPad(const PlantConfig &config)
{
}

bool NurturerLogic::isSunnyHour(const PlantConfig &config)
{
    int currentHour = clock.getHour();
    int half = config.sunnyHours / 2;
    bool afternoon = currentHour > 12;

    if (config.sunnyHours % 2 == 0)
    {
        return afternoon ? currentHour - 12 <= half : 12 - currentHour <= half;
    }
    else
    {
        return afternoon ? currentHour - 12 <= half : 12 - currentHour <= half + 1;
    }
}
