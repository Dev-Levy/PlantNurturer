#pragma once
#include <Arduino.h>
#include <config.h>

class ActuatorManager
{
public:
    ActuatorManager();
    void togglePump();
    void toggleLight();
    void toggleFan();

private:
    struct
    {
        uint8_t pumpOn : 1;
        uint8_t lightOn : 1;
        uint8_t fanOn : 1;
    } state;

    void turnOnPump();
    void turnOffPump();
    void turnOnLight();
    void turnOffLight();
    void turnOnFan();
    void turnOffFan();
};
