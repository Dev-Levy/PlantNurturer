#pragma once

#include <Arduino.h>
#include <config.h>
#include "RTClib.h"

class TimeManager
{
public:
    TimeManager();
    bool begin();
    void updateTime();

    inline uint32_t getUnixNow() { return rtc.now().unixtime(); }
    inline uint8_t getHour() { return now.hour(); }
    inline uint8_t getMinute() { return now.minute(); }
    inline uint8_t getSecond() { return now.second(); }

    inline uint8_t getDay() { return now.day(); }
    inline uint8_t getMonth() { return now.month(); }
    inline uint8_t getYear() { return now.year(); }

    uint8_t getGrowthWeek(uint32_t currentTime);
    void resetWeekCounter(uint32_t currentTime);
    void getGrowthStartTime(uint32_t &startTime) { startTime = growthStartTime; }
    void setGrowthStartTime(uint32_t startTime) { growthStartTime = startTime; }

private:
    uint32_t growthStartTime;
    RTC_DS1307 rtc;
    DateTime now;
};