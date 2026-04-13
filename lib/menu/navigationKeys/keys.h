#pragma once
#include <Arduino.h>
#include <config.h>

enum class KeyPress : uint8_t
{
    NONE = 0,
    UP = 1,
    DOWN = 2,
    SELECT = 3
};

KeyPress readKeys();