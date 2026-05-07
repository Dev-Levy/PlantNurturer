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
        pumpWaitTime = millis();
    }
    else
    {
        turnOnPump();
        pumpStart = millis();
    }
    state.pumpOn = !state.pumpOn;
}

void ActuatorManager::toggleLight()
{
    if (state.lightOn)
    {
        turnOffLight();
        lightWaitTime = millis();
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
        fanWaitTime = millis();
    }
    else
    {
        turnOnFan();
    }
    state.fanOn = !state.fanOn;
}

void ActuatorManager::togglePad()
{
    if (state.padOn)
    {
        turnOffPad();
        padWaitTime = millis();
    }
    else
    {
        turnOnPad();
    }
    state.padOn = !state.padOn;
}

void ActuatorManager::turnOnPump()
{
    Serial.println(F("Pump turned ON"));
    digitalWrite(PUMP, LOW);
}

void ActuatorManager::turnOffPump()
{
    Serial.println(F("Pump turned OFF"));
    digitalWrite(PUMP, HIGH);
}

void ActuatorManager::turnOnLight()
{
    Serial.println(F("Light turned ON"));
    digitalWrite(LIGHT, LOW);
}

void ActuatorManager::turnOffLight()
{
    Serial.println(F("Light turned OFF"));
    digitalWrite(LIGHT, HIGH);
}

void ActuatorManager::turnOnFan()
{
    Serial.println(F("Fan turned ON"));
    digitalWrite(FAN, LOW);
}

void ActuatorManager::turnOffFan()
{
    Serial.println(F("Fan turned OFF"));
    digitalWrite(FAN, HIGH);
}

void ActuatorManager::turnOnPad()
{
    Serial.println(F("Pad turned ON"));
    digitalWrite(PAD, LOW);
}

void ActuatorManager::turnOffPad()
{
    Serial.println(F("Pad turned OFF"));
    digitalWrite(PAD, HIGH);
}
