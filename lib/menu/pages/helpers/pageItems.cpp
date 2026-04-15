#include <Arduino.h>
#include "menuSystem.h"
#include "pages/pages.h"
#include "itemLabels.cpp"
#include "forwardDeclarations.h"

const MenuItem homeItems[] PROGMEM = {
    {(const __FlashStringHelper *)labelMainPlant, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelEnterMain, &mainPage, nullptr, nullptr},
};
const MenuItem mainItems[] PROGMEM = {
    {(const __FlashStringHelper *)labelPlants, &plantsPage, nullptr, nullptr},
    {(const __FlashStringHelper *)labelSensors, &sensorPage, nullptr, nullptr},
    {(const __FlashStringHelper *)labelActuators, &actuatorPage, nullptr, nullptr},
    {(const __FlashStringHelper *)labelBack, &homePage, nullptr, nullptr},
};
const MenuItem plantsItems[] PROGMEM = {
    {(const __FlashStringHelper *)labelSelect, &selectPlantPage, nullptr, nullptr},
    {(const __FlashStringHelper *)labelSelect, &selectPlantPage, nullptr, nullptr},
    {(const __FlashStringHelper *)labelSelect, &selectPlantPage, nullptr, nullptr},
    {(const __FlashStringHelper *)labelBack, &mainPage, nullptr, nullptr},
};
const MenuItem selectPlantItems[] PROGMEM = {
    {(const __FlashStringHelper *)labelTomato, nullptr, MenuSystem::plantSelectionCallback, (void *)&tomatoPage},
    {(const __FlashStringHelper *)labelChili, nullptr, MenuSystem::plantSelectionCallback, (void *)&chiliPage},
    {(const __FlashStringHelper *)labelMint, nullptr, MenuSystem::plantSelectionCallback, (void *)&mintPage},
    {(const __FlashStringHelper *)labelBasil, nullptr, MenuSystem::plantSelectionCallback, (void *)&basilPage},
    {(const __FlashStringHelper *)labelBack, &mainPage, nullptr, nullptr},
};
const MenuItem plantItems[] PROGMEM = {
    {(const __FlashStringHelper *)labelSetting, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelSetMain, nullptr, MenuSystem::plantSetAsMainCallback, nullptr},
    {(const __FlashStringHelper *)labelRemove, nullptr, MenuSystem::plantRemoveCallback, nullptr},
    {(const __FlashStringHelper *)labelBack, &plantsPage, nullptr, nullptr},
};

const MenuItem sensorDataItems[] PROGMEM = {
    {(const __FlashStringHelper *)labelLightSensor, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelAirTempSensor, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelAirHumiditySensor, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelSoilMoistureSensor, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelSoilTempSensor, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelBack, &mainPage, nullptr, nullptr},
};
const MenuItem actuatorActionItems[] PROGMEM = {
    {(const __FlashStringHelper *)labelPump, nullptr, MenuSystem::togglePumpCallBack, &actuators},
    {(const __FlashStringHelper *)labelLight, nullptr, MenuSystem::toggleLightCallBack, &actuators},
    {(const __FlashStringHelper *)labelFan, nullptr, MenuSystem::toggleFanCallBack, &actuators},
    {(const __FlashStringHelper *)labelBack, &mainPage, nullptr, nullptr},
};