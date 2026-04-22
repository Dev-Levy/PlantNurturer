#pragma once
#include <Arduino.h>

// sensor types
#define DHT_TYPE DHT22

// constants
static constexpr uint8_t HOME_PAGE_ITEMS = 1;
static constexpr uint8_t MAIN_PAGE_ITEMS = 4;
static constexpr uint8_t PLANTS_PAGE_ITEMS = 4;
static constexpr uint8_t PLANT_SELECT_PAGE_ITEMS = 5;
static constexpr uint8_t PLANT_PAGE_ITEMS = 7;
static constexpr uint8_t SENSORS_PAGE_ITEMS = 6;
static constexpr uint8_t ACTUATORS_PAGE_ITEMS = 4;

static constexpr uint8_t PLANT_TYPE_COUNT = 4;
static constexpr uint8_t PLANT_COUNT = 3;
static constexpr uint8_t EEPROM_ADDR = 0;

static constexpr uint8_t LINE_HEIGHT = 14;
static constexpr uint8_t PADDING = 2;
static constexpr uint8_t MAX_VISIBLE = 6;

static constexpr uint32_t SECONDS_IN_A_WEEK = 604800UL;

// actuator pins
#define PUMP 2
#define LIGHT 3
#define FAN 4

// display pins
#define TFT_CS 10
#define TFT_RST 8
#define TFT_DC 9

// button pins
#define BUTTON_UP 5
#define BUTTON_DOWN 6
#define BUTTON_SELECT 7

// sensor pins
#define SOIL_MOIST_PIN A1
#define SOIL_TEMP_PIN 12

inline void setupPins()
{
    pinMode(PUMP, OUTPUT);
    pinMode(LIGHT, OUTPUT);
    pinMode(FAN, OUTPUT);
    digitalWrite(PUMP, HIGH);
    digitalWrite(LIGHT, HIGH);
    digitalWrite(FAN, HIGH);

    pinMode(BUTTON_UP, INPUT);
    pinMode(BUTTON_DOWN, INPUT);
    pinMode(BUTTON_SELECT, INPUT);
}