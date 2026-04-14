#pragma once
#include <actuatorManager.h>

extern void togglePumpCallBack(void *ctx);
extern void toggleLightCallBack(void *ctx);
extern void toggleFanCallBack(void *ctx);
extern ActuatorManager actuators;