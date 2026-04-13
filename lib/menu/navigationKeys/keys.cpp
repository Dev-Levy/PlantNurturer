#include "keys.h"

KeyPress readKeys()
{
    if (digitalRead(BUTTON_SELECT) == HIGH)
        return KeyPress::SELECT;
    else if (digitalRead(BUTTON_UP) == HIGH)
        return KeyPress::UP;
    else if (digitalRead(BUTTON_DOWN) == HIGH)
        return KeyPress::DOWN;

    return KeyPress::NONE;
}