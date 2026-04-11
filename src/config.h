#pragma once
#include <Arduino.h>

// sensor types
#define DHT_TYPE DHT22

// constants
const int MAX_MENU_ITEMS = 8;
const int PLANT_TYPE_COUNT = 4;
const int PLANT_PAGES_COUNT = 3;
extern const char *const plantNames[];
const int EEPROM_ADDR = 0;

// pins
const int TFT_CS = 10;
const int TFT_RST = 8;
const int TFT_DC = 9;

const int BUTTON_UP = 5;
const int BUTTON_DOWN = 6;
const int BUTTON_SELECT = 7;

const int SOIL_TEMP_PIN = 2;
const int LDR_PIN = 3;
const int DHT_PIN = 4;
#define SOIL_MOIST_PIN A1

void setupPins();