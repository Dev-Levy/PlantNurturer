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
    void forceTimeSync();

    inline uint32_t getUnixNow() { return isRTCInitialized ? rtc.now().unixtime() : 0; }

    inline uint8_t getHour()
    {
        TEST_CONTENT(return hour;)

        return isRTCInitialized ? now.hour() : 0;
    }

    TEST_CONTENT(inline void setHour(uint8_t h) {
        hour = h;
    })
    TEST_CONTENT(inline void resetMock() {
        hour = now.hour();
    })

    inline uint8_t getMinute() { return isRTCInitialized ? now.minute() : 0; }
    inline uint8_t getSecond() { return isRTCInitialized ? now.second() : 0; }

    inline uint8_t getDay() { return isRTCInitialized ? now.day() : 0; }
    inline uint8_t getMonth() { return isRTCInitialized ? now.month() : 0; }
    inline uint8_t getYear() { return isRTCInitialized ? now.year() : 0; }

    uint8_t getGrowthWeek(uint32_t currentTime);
    void resetGrowthWeek(uint32_t currentTime);
    void getGrowthStartTime(uint32_t &startTime) { startTime = growthStartTime; }
    void setGrowthStartTime(uint32_t startTime) { growthStartTime = startTime; }

private:
    TEST_CONTENT(uint8_t hour = 0;)
    bool isRTCInitialized;
    uint32_t growthStartTime;
    RTC_DS1307 rtc;
    DateTime now;
};