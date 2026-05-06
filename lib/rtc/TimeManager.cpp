#include "timeManager.h"

TimeManager::TimeManager() {}

bool TimeManager::begin()
{
    // ################################################
    growthStartTime = DateTime(2026, 4, 1, 12, 0, 0).unixtime();
    updateTime();
    // ################################################

    if (!rtc.begin())
    {
        return false;
    }
    if (!rtc.isrunning())
    {
        rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    isRTCInitialized = true;
    return true;
}

void TimeManager::updateTime()
{
    now = rtc.now();
}

uint8_t TimeManager::getGrowthWeek(uint32_t currentTime)
{
    if (growthStartTime == 0 || currentTime < growthStartTime)
    {
        return 1;
    }

    uint32_t diffInSeconds = currentTime - growthStartTime;

    return isRTCInitialized ? (diffInSeconds / SECONDS_IN_A_WEEK) + 1 : 0;
}

void TimeManager::resetWeekCounter(uint32_t currentTime)
{
    growthStartTime = currentTime;
}