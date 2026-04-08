#pragma once

class IActuatorActions
{
public:
    virtual ~IActuatorActions() = default;
    virtual void togglePump() = 0;
    virtual void toggleLight() = 0;
    virtual void toggleFan() = 0;
};