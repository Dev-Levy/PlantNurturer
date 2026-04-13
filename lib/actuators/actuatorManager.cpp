#include "actuatorManager.h"

ActuatorManager::ActuatorManager()
{
    state.pumpOn = 0;
    state.lightOn = 0;
    state.fanOn = 0;
}

void ActuatorManager::togglePump()
{
    if (state.pumpOn)
    {
        turnOffPump();
    }
    else
    {
        turnOnPump();
    }
    state.pumpOn = !state.pumpOn;
}

void ActuatorManager::toggleLight()
{
    if (state.lightOn)
    {
        turnOffLight();
    }
    else
    {
        turnOnLight();
    }
    state.lightOn = !state.lightOn;
}

void ActuatorManager::toggleFan()
{
    if (state.fanOn)
    {
        turnOffFan();
    }
    else
    {
        turnOnFan();
    }
    state.fanOn = !state.fanOn;
}

void ActuatorManager::turnOnPump()
{
    Serial.println(F("Pump turned ON"));
}

void ActuatorManager::turnOffPump()
{
    Serial.println(F("Pump turned OFF"));
}

void ActuatorManager::turnOnLight()
{
    Serial.println(F("Light turned ON"));
}

void ActuatorManager::turnOffLight()
{
    Serial.println(F("Light turned OFF"));
}

void ActuatorManager::turnOnFan()
{
    Serial.println(F("Fan turned ON"));
}

void ActuatorManager::turnOffFan()
{
    Serial.println(F("Fan turned OFF"));
}