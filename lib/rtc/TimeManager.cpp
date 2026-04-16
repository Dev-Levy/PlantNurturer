#include "TimeManager.h"

TimeManager::TimeManager() {}

bool TimeManager::begin()
{
    // ################################################
    // ################################################
    // ################################################

    // TESTING
    growthStartTime = DateTime(2026, 4, 1, 12, 0, 0).unixtime();
    updateTime();
    // DELETE LATER

    // ################################################
    // ################################################
    // ################################################

    if (!rtc.begin())
    {
        return false;
    }
    if (!rtc.isrunning())
    {
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

uint8_t TimeManager::getGrowthWeek(uint32_t currentTime)
{
    if (growthStartTime == 0 || currentTime < growthStartTime)
    {
        return 1;
    }

    uint32_t diffInSeconds = currentTime - growthStartTime;

    return (diffInSeconds / SECONDS_IN_A_WEEK) + 1;
}

void TimeManager::resetWeekCounter(uint32_t currentTime)
{
    growthStartTime = currentTime;
}

long TimeManager::getUnixNow()
{
    return rtc.now().unixtime();
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