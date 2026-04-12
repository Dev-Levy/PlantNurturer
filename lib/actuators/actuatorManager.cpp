#include "actuatorManager.h"

ActuatorManager::ActuatorManager()
{
}

void ActuatorManager::togglePump()
{
    if (pumpOn)
    {
        turnOffPump();
    }
    else
    {
        turnOnPump();
    }
    pumpOn = !pumpOn;
}

void ActuatorManager::toggleLight()
{
    if (lightOn)
    {
        turnOffLight();
    }
    else
    {
        turnOnLight();
    }
    lightOn = !lightOn;
}

void ActuatorManager::toggleFan()
{
    if (fanOn)
    {
        turnOffFan();
    }
    else
    {
        turnOnFan();
    }
    fanOn = !fanOn;
}

void ActuatorManager::turnOnPump()
{
    Serial.println("Pump turned ON");
}

void ActuatorManager::turnOffPump()
{
    Serial.println("Pump turned OFF");
}

void ActuatorManager::turnOnLight()
{
    Serial.println("Light turned ON");
}

void ActuatorManager::turnOffLight()
{
    Serial.println("Light turned OFF");
}

void ActuatorManager::turnOnFan()
{
    Serial.println("Fan turned ON");
}

void ActuatorManager::turnOffFan()
{
    Serial.println("Fan turned OFF");
}