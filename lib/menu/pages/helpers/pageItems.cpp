#include <Arduino.h>
#include "pages/pages.h"
#include "itemLabels.cpp"

const MenuItem homeItems[] PROGMEM = {
    {(const __FlashStringHelper *)labelEnterMain, &mainPage, nullptr, nullptr},
};
const MenuItem mainItems[] PROGMEM = {
    {(const __FlashStringHelper *)labelPlants, &plantsPage, nullptr, nullptr},
    {(const __FlashStringHelper *)labelSensors, &sensorPage, nullptr, nullptr},
    {(const __FlashStringHelper *)labelActuators, &actuatorPage, nullptr, nullptr},
    {(const __FlashStringHelper *)labelBack, &homePage, nullptr, nullptr},
};
const MenuItem plantsItems[] PROGMEM = {
    {(const __FlashStringHelper *)labelPlant1, &selectPlantPage, nullptr, nullptr},
    {(const __FlashStringHelper *)labelPlant2, &selectPlantPage, nullptr, nullptr},
    {(const __FlashStringHelper *)labelPlant3, &selectPlantPage, nullptr, nullptr},
    {(const __FlashStringHelper *)labelBack, &mainPage, nullptr, nullptr},
};
const MenuItem selectPlantItems[] PROGMEM = {
    {(const __FlashStringHelper *)labelTomato, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelMint, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelBasil, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelChili, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelBack, &mainPage, nullptr, nullptr},
};
const MenuItem plant1Items[] PROGMEM = {
    {(const __FlashStringHelper *)labelSetting, nullptr, nullptr, nullptr}};
const MenuItem plant2Items[] PROGMEM = {
    {(const __FlashStringHelper *)labelSetting, nullptr, nullptr, nullptr}};
const MenuItem plant3Items[] PROGMEM = {
    {(const __FlashStringHelper *)labelSetting, nullptr, nullptr, nullptr}};

const MenuItem sensorDataItems[] PROGMEM = {
    {(const __FlashStringHelper *)labelLightSensor, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelAirTempSensor, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelAirHumiditySensor, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelSoilMoistureSensor, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelSoilTempSensor, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelBack, &mainPage, nullptr, nullptr},
};
const MenuItem actuatorActionItems[] PROGMEM = {
    {(const __FlashStringHelper *)labelPump, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelLight, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelFan, nullptr, nullptr, nullptr},
    {(const __FlashStringHelper *)labelBack, &mainPage, nullptr, nullptr},
};