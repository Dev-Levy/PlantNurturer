#include "nurturerLogic.h"

NurturerLogic::NurturerLogic(TimeManager &clock, SensorManager &sensor, ActuatorManager &actuator, PlantManager &plant)
    : clock(clock), sensor(sensor), actuator(actuator), plant(plant), data(sensor.readAll())
{
}

void NurturerLogic::control(const PlantConfig &config)
{
    if (config.idx != lastConfigIndex)
    {
        lastConfigIndex = config.idx;

        DEBUG_PRINT(F("Sunny hours: "));
        DEBUG_PRINT(config.sunnyHours);
        DEBUG_PRINTLN(F(" h"));

        DEBUG_PRINT(F("Light limit: "));
        DEBUG_PRINT(LUX_LIMIT);
        DEBUG_PRINTLN(F(" lux"));

        DEBUG_PRINT(F("Water limit: "));
        DEBUG_PRINT(config.waterLimit);
        DEBUG_PRINTLN(F(" %"));

        DEBUG_PRINT(F("Water ml: "));
        DEBUG_PRINTLN(config.waterMl);

        DEBUG_PRINT(F("Ideal temp: "));
        DEBUG_PRINTLN(config.idealTemp);
        DEBUG_PRINT(F("Ideal soil temp: "));
        DEBUG_PRINTLN(config.idealSoilTemp);

        DEBUG_PRINTLN(F("--------------------"));
    }

    if (millis() - lastSensorRefresh > SENSOR_READ_WAIT_TIME * 1000UL)
    {
        sensor.readAll();
        lastSensorRefresh = millis();
    }

    controlPump(config);
    controlLight(config);
    controlFan(config);
    controlPad(config);
}

void NurturerLogic::controlPump(const PlantConfig &config)
{
    unsigned long now = millis();
    const uint8_t wateringMillis = plant.getWateringMs(config.waterMl);
    const unsigned long wateringDurationMs = (unsigned long)wateringMillis;

    bool pumpOn = actuator.state.pumpOn;
    bool shouldWater = data.soilMoisture < config.waterLimit;
    bool canToggle = now - actuator.pumpWaitTime >= PUMP_COOLDOWN_IN_SECONDS * 1000UL;
    bool wateredEnough = pumpOn && now - actuator.pumpStart >= wateringDurationMs;

    bool shouldTurnOn = !pumpOn && shouldWater && canToggle;
    bool shouldTurnOff = (pumpOn && wateredEnough) || (pumpOn && !shouldWater);

    if (millis() - lastSensorRefresh > SENSOR_READ_WAIT_TIME * 1000UL)
    {
        DEBUG_PRINT(pumpOn ? F("Pump is ON. ") : F("Pump is OFF. "));
        DEBUG_PRINT(shouldWater ? F("Should water. ") : F("Should NOT water. "));
        DEBUG_PRINTLN(canToggle ? F("Can toggle. ") : F("Cannot toggle. "));
    }

    if (shouldTurnOff)
    {
        actuator.togglePump();
    }
    else if (shouldTurnOn)
    {
        actuator.togglePump();
        actuator.pumpStart = now;
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

    if (millis() - lastSensorRefresh > SENSOR_READ_WAIT_TIME * 1000UL)
    {
        DEBUG_PRINT(lightOn ? F("Light is ON. ") : F("Light is OFF. "));
        DEBUG_PRINT(isThereEnoughLight ? F("There is enough light. ") : F("There is NOT enough light. "));
        DEBUG_PRINT(shouldHaveLight ? F("Should have light. ") : F("Should NOT have light. "));
        DEBUG_PRINTLN(canToggle ? F("Can toggle. ") : F("Cannot toggle. "));
    }
    if (shouldTurnOff)
    {
        actuator.toggleLight();
    }
    else if (shouldTurnOn)
    {
        actuator.toggleLight();
    }
}

void NurturerLogic::controlFan(const PlantConfig &config)
{
    bool fanOn = actuator.state.fanOn;

    bool isTooHot = data.airTemp > (config.idealTemp + (uint16_t)(CELSIUS_DIFF_ALLOWED * 10));
    bool isTooCold = data.airTemp < (config.idealTemp - (uint16_t)(CELSIUS_DIFF_ALLOWED * 10));

    bool isTooHumid = data.airHumidity > (config.idealHumidity + (uint16_t)(HUMIDITY_DIFF_ALLOWED * 10));
    bool isHumidityIdeal = data.airHumidity <= config.idealHumidity;

    bool canToggle = millis() - actuator.fanWaitTime >= FAN_COOLDOWN_IN_SECONDS * 1000UL;

    bool shouldTurnOn = !fanOn && (isTooHot || isTooHumid) && canToggle;
    bool shouldTurnOff = fanOn && (isTooCold && isHumidityIdeal) && canToggle;

    if (millis() - lastSensorRefresh > SENSOR_READ_WAIT_TIME * 1000UL)
    {
        DEBUG_PRINT(fanOn ? F("Fan is ON. ") : F("Fan is OFF. "));
        DEBUG_PRINT(isTooHot ? F("It is too hot. ") : F("It is not too hot. "));
        DEBUG_PRINT(isTooCold ? F("It is too cold. ") : F("It is not too cold. "));
        DEBUG_PRINTLN(canToggle ? F("Can toggle. ") : F("Cannot toggle. "));
    }

    if (shouldTurnOff)
    {
        actuator.toggleFan();
    }
    else if (shouldTurnOn)
    {
        actuator.toggleFan();
    }
}

void NurturerLogic::controlPad(const PlantConfig &config)
{
    bool padOn = actuator.state.padOn;
    bool isTooCold = data.soilTemp < (config.idealSoilTemp - (uint16_t)(CELSIUS_DIFF_ALLOWED * 10));
    bool isTooHot = data.soilTemp > (config.idealSoilTemp + (uint16_t)(CELSIUS_DIFF_ALLOWED * 10));
    bool canToggle = millis() - actuator.padWaitTime >= HEATING_COOLDOWN_IN_SECONDS * 1000UL;

    bool shouldTurnOn = !padOn && isTooCold && canToggle;
    bool shouldTurnOff = padOn && isTooHot && canToggle;

    // if (millis() - lastSensorRefresh > SENSOR_READ_WAIT_TIME * 1000UL)
    // {
    //     DEBUG_PRINT(padOn ? F("Pad is ON. ") : F("Pad is OFF. "));
    //     DEBUG_PRINT(isTooCold ? F("Soil is too cold. ") : F("Soil is not too cold. "));
    //     DEBUG_PRINT(isTooHot ? F("Soil is too hot. ") : F("Soil is not too hot. "));
    //     DEBUG_PRINTLN(canToggle ? F("Can toggle. ") : F("Cannot toggle. "));
    // }

    if (shouldTurnOff)
    {
        actuator.togglePad();
    }
    else if (shouldTurnOn)
    {
        actuator.togglePad();
    }
}

bool NurturerLogic::isSunnyHour(const PlantConfig &config)
{
    int currentHour = clock.getHour();
    int startHour = 12 - (config.sunnyHours / 2);
    int endHour = startHour + config.sunnyHours;

    return (currentHour >= startHour && currentHour < endHour);
}
