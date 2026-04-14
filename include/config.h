#pragma once
#include <Arduino.h>

// sensor types
#define DHT_TYPE DHT22

// constants
constexpr uint8_t HOME_PAGE_ITEMS = 1;
constexpr uint8_t MAIN_PAGE_ITEMS = 4;
constexpr uint8_t PLANTS_PAGE_ITEMS = 4;
constexpr uint8_t PLANT_SELECT_PAGE_ITEMS = 5;
constexpr uint8_t PLANT_PAGE_ITEMS = 2;
constexpr uint8_t SENSORS_PAGE_ITEMS = 6;
constexpr uint8_t ACTUATORS_PAGE_ITEMS = 4;

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

inline void setupPins()
{
    pinMode(BUTTON_UP, INPUT);
    pinMode(BUTTON_DOWN, INPUT);
    pinMode(BUTTON_SELECT, INPUT);
}