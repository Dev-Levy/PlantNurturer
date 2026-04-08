#include "config.h"
#include "Arduino.h"

const char p_tomato[] PROGMEM = "Tomato";
const char p_basil[] PROGMEM = "Basil";
const char p_mint[] PROGMEM = "Mint";
const char p_chili[] PROGMEM = "Chili";

const char *const plantNames[] PROGMEM = {p_tomato, p_basil, p_mint, p_chili};

void setupPins()
{
    pinMode(BUTTON_UP, INPUT);
    pinMode(BUTTON_DOWN, INPUT);
    pinMode(BUTTON_SELECT, INPUT);
}