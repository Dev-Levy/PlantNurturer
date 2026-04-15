#include "TimeManager.h"

TimeManager::TimeManager() {}

bool TimeManager::begin()
{
    if (!rtc.begin())
    {
        return false;
    }

    if (!rtc.isrunning())
    {
        // Sets the time to the moment the code was compiled
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    return true;
}

void TimeManager::updateTime()
{
    now = rtc.now();
}

void TimeManager::forceSetTime(int year, int month, int day, int hour, int minute, int second)
{
    rtc.adjust(DateTime(year, month, day, hour, minute, second));
}

const char *TimeManager::getFormattedTime()
{
    snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", now.hour(), now.minute(), now.second());

    return buffer;
}

const char *TimeManager::getFormattedDate()
{
    snprintf(buffer, sizeof(buffer), "%04d-%02d-%02d", now.year(), now.month(), now.day());

    return buffer;
}