#pragma once
#include <Arduino.h>

class ITimeActions
{
public:
    virtual ~ITimeActions() = default;
    virtual bool begin() = 0;
    virtual void updateTime() = 0;
    virtual void forceSetTime(int year, int month, int day, int hour, int minute, int second) = 0;
    virtual uint8_t getGrowthWeek(uint32_t currentTime) = 0;
    virtual void resetWeekCounter(uint32_t currentTime) = 0;

    virtual long getUnixNow() = 0;

    virtual uint8_t getHour() = 0;
    virtual uint8_t getMinute() = 0;
    virtual uint8_t getSecond() = 0;

    virtual uint8_t getDay() = 0;
    virtual uint8_t getMonth() = 0;
    virtual uint8_t getYear() = 0;

    virtual const char *getFormattedTime();
    virtual const char *getFormattedDate();
};