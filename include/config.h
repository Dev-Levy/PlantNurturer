#pragma once
#include <Arduino.h>

// sensor types
#define DHT_TYPE DHT22

// constants
constexpr int MAX_MENU_ITEMS = 8;
constexpr int PLANT_TYPE_COUNT = 4;
constexpr int PLANT_PAGES_COUNT = 3;
constexpr int EEPROM_ADDR = 0;

// pins
constexpr int TFT_CS = 10;
constexpr int TFT_RST = 8;
constexpr int TFT_DC = 9;

constexpr int BUTTON_UP = 5;
constexpr int BUTTON_DOWN = 6;
constexpr int BUTTON_SELECT = 7;

constexpr int SOIL_TEMP_PIN = 2;
constexpr int LDR_PIN = 3;
constexpr int DHT_PIN = 4;
#define SOIL_MOIST_PIN A1

// Plant names in PROGMEM
const char PROGMEM p_tomato[] = "Tomato";
const char PROGMEM p_basil[] = "Basil";
const char PROGMEM p_mint[] = "Mint";
const char PROGMEM p_chili[] = "Chili";

const char *const PROGMEM plantNames[] = {p_tomato, p_basil, p_mint, p_chili};

// Inline setup function
inline void setupPins()
{
    pinMode(BUTTON_UP, INPUT);
    pinMode(BUTTON_DOWN, INPUT);
    pinMode(BUTTON_SELECT, INPUT);
}