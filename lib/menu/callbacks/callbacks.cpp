#include "callbacks.h"

void togglePumpCallBack(void *ctx)
{
    if (ctx)
    {
        IActuatorActions *actuators = (IActuatorActions *)ctx;
        actuators->togglePump();
    }
}

void toggleLightCallBack(void *ctx)
{
    if (ctx)
    {
        IActuatorActions *actuators = (IActuatorActions *)ctx;
        actuators->toggleLight();
    }
}

void toggleFanCallBack(void *ctx)
{
    if (ctx)
    {
        IActuatorActions *actuators = (IActuatorActions *)ctx;
        actuators->toggleFan();
    }
}
