#pragma once

#include <Arduino.h>
#include <config.h>
#include "RTClib.h"

#include "TimeActionsInterface.h"

class TimeManager : public ITimeActions
{
public:
    TimeManager();
    bool begin();
    void updateTime();
    void forceSetTime(int year, int month, int day, int hour, int minute, int second);
    uint8_t getGrowthWeek(uint32_t currentTime);
    void resetWeekCounter(uint32_t currentTime);

    long getUnixNow();

    uint8_t getHour() { return now.hour(); }
    uint8_t getMinute() { return now.minute(); }
    uint8_t getSecond() { return now.second(); }

    uint8_t getDay() { return now.day(); }
    uint8_t getMonth() { return now.month(); }
    uint8_t getYear() { return now.year(); }

    const char *getFormattedTime();
    const char *getFormattedDate();

private:
    uint32_t growthStartTime;
    RTC_DS1307 rtc;
    DateTime now;
    char buffer[12];
};