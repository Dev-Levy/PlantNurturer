#include "keys.h"

int readKeys()
{
    if (digitalRead(BUTTON_UP) == HIGH)
        return 1;
    if (digitalRead(BUTTON_DOWN) == HIGH)
        return 2;
    if (digitalRead(BUTTON_SELECT) == HIGH)
        return 3;
    return 0;
}