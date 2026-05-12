#include "actuatorManager.h"

ActuatorManager::ActuatorManager()
{
    *(uint8_t *)&state = 0;
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
    pumpWaitTime = millis();
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
    lightWaitTime = millis();
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
    fanWaitTime = millis();
    state.fanOn = !state.fanOn;
}

void ActuatorManager::togglePad()
{
    if (state.padOn)
    {
        turnOffPad();
    }
    else
    {
        turnOnPad();
    }
    padWaitTime = millis();
    state.padOn = !state.padOn;
}

void ActuatorManager::turnOnPump()
{
    DEBUG_PRINTLN(F("Pump ON"));
    digitalWrite(PUMP, LOW);
}

void ActuatorManager::turnOffPump()
{
    DEBUG_PRINTLN(F("Pump OFF"));
    digitalWrite(PUMP, HIGH);
}

void ActuatorManager::turnOnLight()
{
    DEBUG_PRINTLN(F("Light ON"));
    digitalWrite(LIGHT, LOW);
}

void ActuatorManager::turnOffLight()
{
    DEBUG_PRINTLN(F("Light OFF"));
    digitalWrite(LIGHT, HIGH);
}

void ActuatorManager::turnOnFan()
{
    DEBUG_PRINTLN(F("Fan ON"));
    digitalWrite(FAN, LOW);
}

void ActuatorManager::turnOffFan()
{
    DEBUG_PRINTLN(F("Fan OFF"));
    digitalWrite(FAN, HIGH);
}

void ActuatorManager::turnOnPad()
{
    DEBUG_PRINTLN(F("Pad ON"));
    digitalWrite(PAD, LOW);
}

void ActuatorManager::turnOffPad()
{
    DEBUG_PRINTLN(F("Pad OFF"));
    digitalWrite(PAD, HIGH);
}
