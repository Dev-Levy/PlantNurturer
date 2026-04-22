#include "nurturerLogic.h"

static unsigned long pumpStart = 0;
static unsigned long pumpWaitTime = 0;
static bool pumpRunning = false;
static bool lightOn = false;
static bool fanRunning = false;

NurturerLogic::NurturerLogic(TimeManager &clock, SensorManager &sensor, ActuatorManager &actuator, PlantManager &plant)
    : clock(clock), sensor(sensor), actuator(actuator), plant(plant)
{
}

void NurturerLogic::control(uint8_t plantIndex)
{
    data = sensor.readAll();
    plant.getPlantConfig(plantIndex);

    const uint8_t wateringSeconds = plant.getWateringSeconds(currentPlantConfig.waterMl);
    const unsigned long wateringDurationMs = (unsigned long)wateringSeconds * 1000UL;

    // START watering
    if (!pumpRunning &&
        data.soilMoisture < currentPlantConfig.waterLimit &&
        millis() - pumpWaitTime < PUMP_COOLDOWN_IN_SECONDS * 1000UL)
    {
        Serial.println("Starting pump");

        actuator.togglePump();

        pumpRunning = true;
        pumpStart = millis();
    }

    // STOP watering after lookup duration
    if (pumpRunning &&
        (millis() - pumpStart >= wateringDurationMs))
    {
        Serial.println("Stopping pump");

        actuator.togglePump();

        pumpRunning = false;
        pumpWaitTime = millis();

        Serial.print("Duration: ");
        Serial.println(pumpStart - millis());
    }

    // START light
    if (!lightOn &&
        !data.light)
    {
        Serial.println("Starting light");

        actuator.toggleLight();
        lightOn = true;
    }

    // STOP light
    if (lightOn &&
        data.light)
    {
        Serial.println("Stopping light");

        actuator.toggleLight();
        lightOn = false;
    }

    // START fan
    if (!fanRunning &&
        data.airTemp > currentPlantConfig.maxTemp)
    {
        Serial.println("Starting fan");

        actuator.toggleFan();
        fanRunning = true;
    }

    // STOP fan
    if (fanRunning &&
        data.airTemp < currentPlantConfig.minTemp)
    {
        Serial.println("Stopping fan");

        actuator.toggleFan();
        fanRunning = false;
    }
}
