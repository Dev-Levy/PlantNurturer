#include <unity.h>
#include <Arduino.h>
#include "config.h"
#include "sensorManager.h"

SensorManager sensors;

void test_sensor_initialization()
{
    sensors.begin();

    TEST_ASSERT_TRUE_MESSAGE(sensors.state.lightReady, "BH1750 (Light) not detected on I2C");
    TEST_ASSERT_TRUE_MESSAGE(sensors.state.ahtReady, "AHT20 (Temp/Humid) not detected on I2C");
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_sensor_initialization);

    UNITY_END();
}

void loop()
{
}
