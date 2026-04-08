#pragma once
#include <Arduino.h>
#include "actuatorActionsInterface.h"

#include "../config.h"

class ActuatorManager : public IActuatorActions
{
public:
    ActuatorManager();
    void togglePump() override;
    void toggleLight() override;
    void toggleFan() override;

private:
    bool pumpOn = false;
    bool lightOn = false;
    bool fanOn = false;
    void turnOnPump();
    void turnOffPump();
    void turnOnLight();
    void turnOffLight();
    void turnOnFan();
    void turnOffFan();
};
