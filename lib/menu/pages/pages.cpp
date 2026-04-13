#include "pages.h"

const char titleHome[] PROGMEM = "Home";
const char titleMain[] PROGMEM = "Main Menu";
const char titlePlants[] PROGMEM = "Plants";
const char titlePlant1[] PROGMEM = "Plant1";
const char titlePlant2[] PROGMEM = "Plant2";
const char titlePlant3[] PROGMEM = "Plant3";
const char titleSelectPlants[] PROGMEM = "Plants";
const char titleSensor[] PROGMEM = "Sensor Data";
const char titleActuator[] PROGMEM = "Actuators";

const MenuItem homeItems[] PROGMEM = {};
const MenuItem mainItems[] PROGMEM = {};
const MenuItem plantsItems[] PROGMEM = {};
const MenuItem selectPlantItems[] PROGMEM = {};
const MenuItem plant1Items[] PROGMEM = {};
const MenuItem plant2Items[] PROGMEM = {};
const MenuItem plant3Items[] PROGMEM = {};
const MenuItem sensorDataItems[] PROGMEM = {};
const MenuItem actuatorActionItems[] PROGMEM = {};

const MenuPage homePage PROGMEM = {(const __FlashStringHelper *)titleHome, homeItems, 1, nullptr};
const MenuPage mainPage PROGMEM = {(const __FlashStringHelper *)titleMain, mainItems, 4, &homePage};

const MenuPage plantsPage PROGMEM = {(const __FlashStringHelper *)titlePlants, plantsItems, 4, &mainPage};
const MenuPage selectPlantPage PROGMEM = {(const __FlashStringHelper *)titleSelectPlants, selectPlantItems, 4, &plantsPage};
const MenuPage plant1Page PROGMEM = {(const __FlashStringHelper *)titlePlant1, plant1Items, 1, &plantsPage};
const MenuPage plant2Page PROGMEM = {(const __FlashStringHelper *)titlePlant2, plant2Items, 1, &plantsPage};
const MenuPage plant3Page PROGMEM = {(const __FlashStringHelper *)titlePlant3, plant3Items, 1, &plantsPage};

const MenuPage sensorPage PROGMEM = {(const __FlashStringHelper *)titleSensor, sensorDataItems, 5, &mainPage};
const MenuPage actuatorPage PROGMEM = {(const __FlashStringHelper *)titleActuator, actuatorActionItems, 4, &mainPage};