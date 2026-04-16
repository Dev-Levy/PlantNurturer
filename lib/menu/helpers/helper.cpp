#include "helper.h"

const char *getMonthName(uint8_t month)
{
    static const char *months[] = {
        "Inv",
        "Jan", "Feb", "Mar",
        "Apr", "May", "Jun",
        "Jul", "Aug", "Sep",
        "Oct", "Nov", "Dec"};

    if (month > 12)
    {
        return months[0];
    }

    return months[month];
}