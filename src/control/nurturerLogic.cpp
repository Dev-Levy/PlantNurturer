#include "nurturerLogic.h"

static unsigned long lastSensorRefresh = 0;
static unsigned long lastConfigIndex = 99;

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

    if (millis() - lastSensorRefresh > SENSOR_READ_WAIT_TIME * 1000UL)
    {
        data = sensor.readAll();
        lastSensorRefresh = millis();
    }

    controlPump(config);
    controlLight(config);
    controlFan(config);
    controlPad(config);
}

void NurturerLogic::controlPump(const PlantConfig &config)
{
    const uint8_t wateringSeconds = plant.getWateringSeconds(config.waterMl);
    const unsigned long wateringDurationMs = (unsigned long)wateringSeconds * 1000UL;

    if (!actuator.state.pumpOn && data.soilMoisture < config.waterLimit &&
        (millis() - actuator.pumpWaitTime >= PUMP_COOLDOWN_IN_SECONDS * 1000UL))
    {
        actuator.togglePump();
    }

    if (actuator.state.pumpOn &&
        (millis() - actuator.pumpStart >= wateringDurationMs))
    {
        actuator.togglePump();
    }
}

void NurturerLogic::controlLight(const PlantConfig &config)
{
    if (!actuator.state.lightOn && !data.light && isSunnyHour(config) &&
        (millis() - actuator.lightWaitTime >= LIGHT_COOLDOWN_IN_SECONDS * 1000UL))
    {
        actuator.toggleLight();
    }

    if ((!isSunnyHour(config) && actuator.state.lightOn) ||
        ((actuator.state.lightOn && data.light) && (millis() - actuator.lightWaitTime >= LIGHT_COOLDOWN_IN_SECONDS * 1000UL)))
    {
        actuator.toggleLight();
    }
}

void NurturerLogic::controlFan(const PlantConfig &config)
{
    if (!actuator.state.fanOn && (float)data.airTemp / 10 > (float)config.idealTemp / 10 + 1 &&
        (millis() - actuator.fanWaitTime >= FAN_COOLDOWN_IN_SECONDS * 1000UL))
    {
        actuator.toggleFan();
    }

    if (actuator.state.fanOn && (float)data.airTemp / 10 < (float)config.idealTemp / 10 - 1 &&
        (millis() - actuator.fanWaitTime >= FAN_COOLDOWN_IN_SECONDS * 1000UL))
    {
        actuator.toggleFan();
    }
}

void NurturerLogic::controlPad(const PlantConfig &config)
{
    if (!actuator.state.padOn && (float)data.soilTemp / 10 > (float)config.idealSoilTemp / 10 - 1 &&
        millis() - actuator.padWaitTime >= HEATING_COOLDOWN_IN_SECONDS * 1000UL)
    {
        actuator.togglePad();
    }

    if (actuator.state.padOn && (float)data.soilTemp / 10 < (float)config.idealSoilTemp / 10 + 1 &&
        millis() - actuator.padWaitTime >= HEATING_COOLDOWN_IN_SECONDS * 1000UL)
    {
        actuator.togglePad();
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
        return afternoon ? currentHour - 12 <= half + 1 : 12 - currentHour <= half;
    }
}
