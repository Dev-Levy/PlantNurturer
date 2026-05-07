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
    void togglePad();

    unsigned long pumpStart = 0;
    unsigned long pumpWaitTime = 0;
    unsigned long lightWaitTime = 0;
    unsigned long fanWaitTime = 0;
    unsigned long padWaitTime = 0;
    struct
    {
        uint8_t pumpOn : 1;
        uint8_t lightOn : 1;
        uint8_t fanOn : 1;
        uint8_t padOn : 1;
    } state;

private:
    void turnOnPump();
    void turnOffPump();
    void turnOnLight();
    void turnOffLight();
    void turnOnFan();
    void turnOffFan();
    void turnOnPad();
    void turnOffPad();
};
