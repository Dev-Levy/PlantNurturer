#include <config.h>
#include "pages.h"
#include "helpers/titles.cpp"
#include "helpers/pageItems.cpp"

const MenuPage homePage PROGMEM = {(const __FlashStringHelper *)titleHome, homeItems, HOME_PAGE_ITEMS, nullptr};
const MenuPage mainPage PROGMEM = {(const __FlashStringHelper *)titleMain, mainItems, MAIN_PAGE_ITEMS, &homePage};

const MenuPage plantsPage PROGMEM = {(const __FlashStringHelper *)titlePlants, plantsItems, PLANTS_PAGE_ITEMS, &mainPage};
const MenuPage selectPlantPage PROGMEM = {(const __FlashStringHelper *)titleSelectPlants, selectPlantItems, PLANT_SELECT_PAGE_ITEMS, &plantsPage};
const MenuPage plant1Page PROGMEM = {(const __FlashStringHelper *)titlePlant1, plant1Items, PLANT_PAGE_ITEMS, &plantsPage};
const MenuPage plant2Page PROGMEM = {(const __FlashStringHelper *)titlePlant2, plant2Items, PLANT_PAGE_ITEMS, &plantsPage};
const MenuPage plant3Page PROGMEM = {(const __FlashStringHelper *)titlePlant3, plant3Items, PLANT_PAGE_ITEMS, &plantsPage};

const MenuPage sensorPage PROGMEM = {(const __FlashStringHelper *)titleSensor, sensorDataItems, SENSORS_PAGE_ITEMS, &mainPage};
const MenuPage actuatorPage PROGMEM = {(const __FlashStringHelper *)titleActuator, actuatorActionItems, ACTUATORS_PAGE_ITEMS, &mainPage};