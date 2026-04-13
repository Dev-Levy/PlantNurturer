#pragma once
#include <Arduino.h>

// sensor types
#define DHT_TYPE DHT22

// constants
constexpr uint8_t PLANT_TYPE_COUNT = 4;
constexpr uint8_t PLANT_COUNT = 3;
constexpr uint8_t EEPROM_ADDR = 0;

// pins
constexpr uint8_t TFT_CS = 10;
constexpr uint8_t TFT_RST = 8;
constexpr uint8_t TFT_DC = 9;

constexpr uint8_t BUTTON_UP = 5;
constexpr uint8_t BUTTON_DOWN = 6;
constexpr uint8_t BUTTON_SELECT = 7;

constexpr uint8_t SOIL_TEMP_PIN = 2;
constexpr uint8_t LDR_PIN = 3;
constexpr uint8_t DHT_PIN = 4;
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