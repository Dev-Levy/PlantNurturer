#include "nurturerLogic.h"

static unsigned long lastSensorRefresh = 0;
static unsigned long lastConfigIndex = 99;
static const uint8_t allowedCelsiusDiff = 1;

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

    bool pumpOn = actuator.state.pumpOn;
    bool shouldWater = data.soilMoisture < config.waterLimit;
    bool canToggle = millis() - actuator.pumpWaitTime >= PUMP_COOLDOWN_IN_SECONDS * 1000UL;
    bool wateredEnough = pumpOn && millis() - actuator.pumpStart >= wateringDurationMs;

    bool shouldTurnOn = !pumpOn && shouldWater && canToggle;
    bool shouldTurnOff = (pumpOn && wateredEnough) || (pumpOn && !shouldWater);

    // if (millis() - lastSensorRefresh > SENSOR_READ_WAIT_TIME * 1000UL)
    // {
    //     Serial.print(pumpOn ? F("Pump is ON. ") : F("Pump is OFF. "));
    //     Serial.print(shouldWater ? F("Should water. ") : F("Should NOT water. "));
    //     Serial.println(canToggle ? F("Can toggle. ") : F("Cannot toggle. "));
    // }

    if (shouldTurnOn)
    {
        actuator.togglePump();
    }
    if (shouldTurnOff)
    {
        actuator.togglePump();
    }
}

void NurturerLogic::controlLight(const PlantConfig &config)
{
    bool lightOn = actuator.state.lightOn;
    bool isThereEnoughLight = data.lightLux > LUX_LIMIT;
    bool shouldHaveLight = isSunnyHour(config);
    bool canToggle = millis() - actuator.lightWaitTime >= LIGHT_COOLDOWN_IN_SECONDS * 1000UL;

    bool shouldTurnOn = !lightOn && !isThereEnoughLight && shouldHaveLight && canToggle;
    bool shouldTurnOff = (lightOn && isThereEnoughLight && canToggle) || (lightOn && !shouldHaveLight);

    // if (millis() - lastSensorRefresh > SENSOR_READ_WAIT_TIME * 1000UL)
    // {
    //     Serial.print(lightOn ? F("Light is ON. ") : F("Light is OFF. "));
    //     Serial.print(isThereEnoughLight ? F("There is enough light. ") : F("There is NOT enough light. "));
    //     Serial.print(shouldHaveLight ? F("Should have light. ") : F("Should NOT have light. "));
    //     Serial.println(canToggle ? F("Can toggle. ") : F("Cannot toggle. "));
    // }

    if (shouldTurnOn)
    {
        actuator.toggleLight();
    }
    if (shouldTurnOff)
    {
        actuator.toggleLight();
    }
}

void NurturerLogic::controlFan(const PlantConfig &config)
{
    bool fanOn = actuator.state.fanOn;
    bool isTooHot = (float)data.airTemp / 10 > (float)config.idealTemp / 10 + allowedCelsiusDiff;
    bool isTooCold = (float)data.airTemp / 10 < (float)config.idealTemp / 10 - allowedCelsiusDiff;
    bool canToggle = millis() - actuator.fanWaitTime >= FAN_COOLDOWN_IN_SECONDS * 1000UL;

    bool shouldTurnOn = !fanOn && isTooHot && canToggle;
    bool shouldTurnOff = fanOn && isTooCold && canToggle;

    // if (millis() - lastSensorRefresh > SENSOR_READ_WAIT_TIME * 1000UL)
    // {
    //     Serial.print(fanOn ? F("Fan is ON. ") : F("Fan is OFF. "));
    //     Serial.print(isTooHot ? F("It is too hot. ") : F("It is not too hot. "));
    //     Serial.print(isTooCold ? F("It is too cold. ") : F("It is not too cold. "));
    //     Serial.println(canToggle ? F("Can toggle. ") : F("Cannot toggle. "));
    // }

    if (shouldTurnOn)
    {
        actuator.toggleFan();
    }
    if (shouldTurnOff)
    {
        actuator.toggleFan();
    }
}

void NurturerLogic::controlPad(const PlantConfig &config)
{
    bool padOn = actuator.state.padOn;
    bool isTooCold = (float)data.soilTemp / 10 < (float)config.idealSoilTemp / 10 - allowedCelsiusDiff;
    bool isTooHot = (float)data.soilTemp / 10 > (float)config.idealSoilTemp / 10 + allowedCelsiusDiff;
    bool canToggle = millis() - actuator.padWaitTime >= HEATING_COOLDOWN_IN_SECONDS * 1000UL;

    bool shouldTurnOn = !padOn && isTooCold && canToggle;
    bool shouldTurnOff = padOn && isTooHot && canToggle;

    // if (millis() - lastSensorRefresh > SENSOR_READ_WAIT_TIME * 1000UL)
    // {
    //     Serial.print(padOn ? F("Pad is ON. ") : F("Pad is OFF. "));
    //     Serial.print(isTooCold ? F("Soil is too cold. ") : F("Soil is not too cold. "));
    //     Serial.print(isTooHot ? F("Soil is too hot. ") : F("Soil is not too hot. "));
    //     Serial.println(canToggle ? F("Can toggle. ") : F("Cannot toggle. "));
    // }

    if (shouldTurnOn)
    {
        actuator.togglePad();
    }
    if (shouldTurnOff)
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
