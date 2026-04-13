#pragma once
#include <Arduino.h>
#include <config.h>

#include "actuatorActionsInterface.h"

class ActuatorManager : public IActuatorActions
{
public:
    ActuatorManager();
    void togglePump() override;
    void toggleLight() override;
    void toggleFan() override;

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
