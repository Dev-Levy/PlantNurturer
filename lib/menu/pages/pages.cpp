#include <config.h>
#include "pages.h"
#include "helpers/titles.cpp"
#include "helpers/pageItems.cpp"

const MenuPage homePage PROGMEM = {(const __FlashStringHelper *)titleHome, homeItems, HOME_PAGE_ITEMS, nullptr};
const MenuPage mainPage PROGMEM = {(const __FlashStringHelper *)titleMain, mainItems, MAIN_PAGE_ITEMS, &homePage};

const MenuPage plantsPage PROGMEM = {(const __FlashStringHelper *)titlePlants, plantsItems, PLANTS_PAGE_ITEMS, &mainPage};
const MenuPage selectPlantPage PROGMEM = {(const __FlashStringHelper *)titleSelectPlants, selectPlantItems, PLANT_SELECT_PAGE_ITEMS, &plantsPage};
const MenuPage tomatoPage PROGMEM = {(const __FlashStringHelper *)titleTomato, plantItems, PLANT_PAGE_ITEMS, &plantsPage};
const MenuPage chiliPage PROGMEM = {(const __FlashStringHelper *)titleChili, plantItems, PLANT_PAGE_ITEMS, &plantsPage};
const MenuPage basilPage PROGMEM = {(const __FlashStringHelper *)titleBasil, plantItems, PLANT_PAGE_ITEMS, &plantsPage};
const MenuPage mintPage PROGMEM = {(const __FlashStringHelper *)titleMint, plantItems, PLANT_PAGE_ITEMS, &plantsPage};

const MenuPage sensorPage PROGMEM = {(const __FlashStringHelper *)titleSensor, sensorDataItems, SENSORS_PAGE_ITEMS, &mainPage};
const MenuPage actuatorPage PROGMEM = {(const __FlashStringHelper *)titleActuator, actuatorActionItems, ACTUATORS_PAGE_ITEMS, &mainPage};