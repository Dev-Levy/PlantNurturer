#include "nurturerLogic.h"

static unsigned long lastSensorRefresh = 0;
static unsigned long lastConfigIndex = 99;

static unsigned long pumpStart = 0;

static unsigned long pumpWaitTime = 0;
static unsigned long lightWaitTime = 0;
static unsigned long fanWaitTime = 0;
static unsigned long padWaitTime = 0;

static struct
{
    uint8_t pumpOn : 1;
    uint8_t lightOn : 1;
    uint8_t fanOn : 1;
    uint8_t padOn : 1;
} state;

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

    if (!state.pumpOn && data.soilMoisture < config.waterLimit &&
        (millis() - pumpWaitTime >= PUMP_COOLDOWN_IN_SECONDS * 1000UL))
    {
        actuator.turnOnPump();
        state.pumpOn = true;
        pumpStart = millis();
    }

    if (state.pumpOn &&
        (millis() - pumpStart >= wateringDurationMs))
    {
        actuator.turnOffPump();
        state.pumpOn = false;
        pumpWaitTime = millis();
    }
}

void NurturerLogic::controlLight(const PlantConfig &config)
{
    if (!state.lightOn && !data.light && isSunnyHour(config) &&
        (millis() - lightWaitTime >= LIGHT_COOLDOWN_IN_SECONDS * 1000UL))
    {
        actuator.turnOnLight();
        state.lightOn = true;
    }

    if (state.lightOn && data.light)
    {
        actuator.turnOffLight();
        lightWaitTime = millis();
        state.lightOn = false;
    }
}

void NurturerLogic::controlFan(const PlantConfig &config)
{
    if (!state.fanOn && (float)data.airTemp / 10 > (float)config.idealTemp / 10 + 5 &&
        (millis() - fanWaitTime >= FAN_COOLDOWN_IN_SECONDS * 1000UL))
    {
        actuator.turnOnFan();
        state.fanOn = true;
    }

    if (state.fanOn && (float)data.airTemp / 10 < (float)config.idealTemp / 10 - 5)
    {
        actuator.turnOffFan();
        fanWaitTime = millis();
        state.fanOn = false;
    }
}

void NurturerLogic::controlPad(const PlantConfig &config)
{
    if (!state.padOn && (float)data.soilTemp / 10 > (float)config.idealSoilTemp / 10 - 5 &&
        millis() - padWaitTime >= HEATING_COOLDOWN_IN_SECONDS * 1000UL)
    {
        actuator.turnOnPad();
        state.padOn = true;
    }

    if (state.padOn && (float)data.soilTemp / 10 < (float)config.idealSoilTemp / 10 + 5)
    {
        actuator.turnOffPad();
        padWaitTime = millis();
        state.padOn = false;
    }
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
