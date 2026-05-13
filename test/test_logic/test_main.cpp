#include <unity.h>
#include "TimeManager.h"
#include "SensorManager.h"
#include "ActuatorManager.h"
#include "PlantManager.h"
#include "nurturerLogic.h"

TimeManager clock;
SensorManager sensors;
ActuatorManager actuators;
PlantManager plants;

NurturerLogic logic(clock, sensors, actuators, plants);

void setUp(void)
{
    clock.resetMock();
    sensors.resetMock();
    actuators.resetStates();
    logic.reset();
}
void tearDown(void)
{
}

#pragma region pump
void test_pump_thresholds()
{
    // Test Case: Just below limit (Should turn ON)
    SensorReading dry;
    dry.soilMoisture = 29;
    sensors.setMockReading(dry);

    PlantConfig config;
    config.waterLimit = 30;
    config.waterMl = 50;

    actuators.state.pumpOn = false;
    actuators.pumpWaitTime = millis() - 70000; // Clear cooldown

    logic.controlPump(config);
    TEST_ASSERT_TRUE(actuators.state.pumpOn);

    // Test Case: Exactly at limit (Should stay OFF)
    actuators.state.pumpOn = false;
    dry.soilMoisture = 30;
    sensors.setMockReading(dry);

    logic.controlPump(config);
    TEST_ASSERT_FALSE(actuators.state.pumpOn);
}
void test_pump_cooldown_safety()
{
    SensorReading veryDry;
    veryDry.soilMoisture = 5;
    sensors.setMockReading(veryDry);

    PlantConfig config;
    config.waterLimit = 50;

    actuators.state.pumpOn = false;
    actuators.pumpWaitTime = millis() - 10000;

    logic.controlPump(config);

    TEST_ASSERT_FALSE(actuators.state.pumpOn);
}
void test_pump_auto_shutoff()
{
    PlantConfig config;
    config.waterLimit = 80;
    config.waterMl = 100; // 2 seconds watering time
    auto wateringMillis = plants.getWateringMs(config.waterMl);

    actuators.state.pumpOn = true;
    actuators.pumpStart = millis() - (wateringMillis + 1000);

    SensorReading dry;
    dry.soilMoisture = 20;
    sensors.setMockReading(dry);

    logic.controlPump(config);

    TEST_ASSERT_FALSE(actuators.state.pumpOn);
}
#pragma endregion

#pragma region light
void test_light_sunny_hours_exact_boundary_start()
{
    PlantConfig config;
    config.sunnyHours = 10; // 12 - 5 => 7:00 - 17:00

    clock.setHour(7);
    SensorReading darkData;
    darkData.lightLux = 10;
    sensors.setMockReading(darkData);
    actuators.state.lightOn = false;
    actuators.lightWaitTime = millis() - (LIGHT_COOLDOWN_IN_SECONDS * 1000UL + 1000);

    logic.controlLight(config);
    TEST_ASSERT_TRUE(actuators.state.lightOn);
}
void test_light_sunny_hours_exact_boundary_end()
{
    PlantConfig config;
    config.sunnyHours = 10; // 12 - 5 => 7:00 - 17:00

    clock.setHour(17);
    SensorReading darkData;
    darkData.lightLux = 10;
    sensors.setMockReading(darkData);
    actuators.state.lightOn = true;
    actuators.lightWaitTime = millis() - (LIGHT_COOLDOWN_IN_SECONDS * 1000UL + 1000);

    logic.controlLight(config);
    TEST_ASSERT_FALSE(actuators.state.lightOn);
}
void test_light_turns_off_when_sun_rises()
{
    PlantConfig config;
    config.sunnyHours = 12;
    clock.setHour(12);

    actuators.state.lightOn = true;
    actuators.lightWaitTime = millis() - 10000;

    SensorReading ligthData;
    ligthData.lightLux = 1000;
    sensors.setMockReading(ligthData);

    logic.controlLight(config);

    TEST_ASSERT_FALSE(actuators.state.lightOn);
}
void test_light_cooldown_prevents_flicker()
{
    PlantConfig config;
    config.sunnyHours = 12;
    clock.setHour(12);

    actuators.state.lightOn = false;
    SensorReading darkData;
    darkData.lightLux = 10;
    sensors.setMockReading(darkData);
    actuators.lightWaitTime = millis();

    logic.controlLight(config);

    TEST_ASSERT_FALSE(actuators.state.lightOn);
}
void test_light_shuts_off_immediately_when_night_falls()
{
    PlantConfig config;
    config.sunnyHours = 12;
    clock.setHour(22);

    actuators.state.lightOn = true;
    SensorReading veryDarkData;
    veryDarkData.lightLux = 0;
    sensors.setMockReading(veryDarkData);
    actuators.lightWaitTime = millis();

    logic.controlLight(config);

    TEST_ASSERT_FALSE(actuators.state.lightOn);
}
#pragma endregion

#pragma region fan
void test_fan_turns_on_high_temp_normal_humidity()
{
    PlantConfig config;
    config.idealTemp = 250;     // 25.0C
    config.idealHumidity = 500; // 50.0%

    SensorReading hotData;
    hotData.airTemp = 250 + (CELSIUS_DIFF_ALLOWED * 10) + 1;
    hotData.airHumidity = 500;

    sensors.setMockReading(hotData);

    actuators.state.fanOn = false;
    actuators.fanWaitTime = millis() - (FAN_COOLDOWN_IN_SECONDS * 1000UL + 1000);

    logic.controlFan(config);
    TEST_ASSERT_TRUE(actuators.state.fanOn);
}
void test_fan_turns_on_high_humidity_normal_temp()
{
    PlantConfig config;
    config.idealTemp = 250;     // 25.0C
    config.idealHumidity = 500; // 50.0%

    SensorReading humidData;
    humidData.airTemp = 250;
    humidData.airHumidity = 500 + (HUMIDITY_DIFF_ALLOWED * 10) + 1;

    sensors.setMockReading(humidData);

    actuators.state.fanOn = false;
    actuators.fanWaitTime = millis() - (FAN_COOLDOWN_IN_SECONDS * 1000UL + 1000);

    logic.controlFan(config);
    TEST_ASSERT_TRUE(actuators.state.fanOn);
}
void test_fan_stays_off_exactly_at_humidity_limit()
{
    PlantConfig config;
    config.idealTemp = 250;     // 25.0C
    config.idealHumidity = 500; // 50.0%

    SensorReading humidLimitData;
    humidLimitData.airTemp = 250;
    humidLimitData.airHumidity = 500 + (HUMIDITY_DIFF_ALLOWED * 10);

    sensors.setMockReading(humidLimitData);

    actuators.state.fanOn = false;
    actuators.fanWaitTime = millis() - (FAN_COOLDOWN_IN_SECONDS * 1000UL + 1000);

    logic.controlFan(config);
    TEST_ASSERT_FALSE(actuators.state.fanOn);
}
void test_fan_stays_on_if_humidity_fixed_but_still_hot()
{
    PlantConfig config;
    config.idealTemp = 250;     // 25.0C
    config.idealHumidity = 500; // 50.0%

    SensorReading notHumidButHotData;
    notHumidButHotData.airTemp = 250 + (CELSIUS_DIFF_ALLOWED * 10) + 1;
    notHumidButHotData.airHumidity = 450;

    sensors.setMockReading(notHumidButHotData);

    actuators.state.fanOn = true;
    actuators.fanWaitTime = millis() - (FAN_COOLDOWN_IN_SECONDS * 1000UL + 1000);

    logic.controlFan(config);
    TEST_ASSERT_TRUE(actuators.state.fanOn);
}
void test_fan_shuts_off_only_when_both_low()
{
    PlantConfig config;
    config.idealTemp = 250;     // 25.0C
    config.idealHumidity = 500; // 50.0%

    SensorReading idealData;
    idealData.airTemp = 250 - (CELSIUS_DIFF_ALLOWED * 10 + 1);
    idealData.airHumidity = 450;

    sensors.setMockReading(idealData);

    actuators.state.fanOn = true;
    actuators.fanWaitTime = millis() - (FAN_COOLDOWN_IN_SECONDS * 1000UL + 1000);

    logic.controlFan(config);
    TEST_ASSERT_FALSE(actuators.state.fanOn);
}
void test_fan_cooldown_prevents_retrigger()
{
    PlantConfig config;
    config.idealTemp = 250;     // 25.0C
    config.idealHumidity = 500; // 50.0%

    SensorReading veryHotHumidData;
    veryHotHumidData.airTemp = 300;
    veryHotHumidData.airHumidity = 800;

    sensors.setMockReading(veryHotHumidData);

    actuators.state.fanOn = false;
    actuators.fanWaitTime = millis();

    logic.controlFan(config);
    TEST_ASSERT_FALSE(actuators.state.fanOn);
}
#pragma endregion

#pragma region pad
void test_pad_turns_on_when_cold()
{
    PlantConfig config;
    config.idealSoilTemp = 250;

    SensorReading coldData;
    coldData.soilTemp = 250 - (CELSIUS_DIFF_ALLOWED * 10) - 1;
    sensors.setMockReading(coldData);

    actuators.state.padOn = false;
    actuators.padWaitTime = millis() - (HEATING_COOLDOWN_IN_SECONDS * 1000UL + 1000);

    logic.controlPad(config);

    TEST_ASSERT_TRUE(actuators.state.padOn);
}
void test_pad_stays_off_in_deadzone()
{
    PlantConfig config;
    config.idealSoilTemp = 250;

    SensorReading bufferData;
    bufferData.soilTemp = 245;
    sensors.setMockReading(bufferData);

    actuators.state.padOn = false;
    actuators.padWaitTime = millis() - 10000;

    logic.controlPad(config);

    TEST_ASSERT_FALSE(actuators.state.padOn);
}
void test_pad_shuts_off_when_hot()
{
    PlantConfig config;
    config.idealSoilTemp = 200;

    SensorReading hotData;
    hotData.soilTemp = 200 + (CELSIUS_DIFF_ALLOWED * 10) + 1;
    sensors.setMockReading(hotData);

    actuators.state.padOn = true;
    actuators.padWaitTime = millis() - 10000;

    logic.controlPad(config);

    TEST_ASSERT_FALSE(actuators.state.padOn);
}
#pragma endregion

void setup()
{
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_pump_thresholds);
    RUN_TEST(test_pump_cooldown_safety);
    RUN_TEST(test_pump_auto_shutoff);

    RUN_TEST(test_light_sunny_hours_exact_boundary_start);
    RUN_TEST(test_light_sunny_hours_exact_boundary_end);
    RUN_TEST(test_light_turns_off_when_sun_rises);
    RUN_TEST(test_light_cooldown_prevents_flicker);
    RUN_TEST(test_light_shuts_off_immediately_when_night_falls);

    RUN_TEST(test_fan_turns_on_high_temp_normal_humidity);
    RUN_TEST(test_fan_turns_on_high_humidity_normal_temp);
    RUN_TEST(test_fan_stays_off_exactly_at_humidity_limit);
    RUN_TEST(test_fan_stays_on_if_humidity_fixed_but_still_hot);
    RUN_TEST(test_fan_shuts_off_only_when_both_low);

    RUN_TEST(test_pad_turns_on_when_cold);
    RUN_TEST(test_pad_stays_off_in_deadzone);
    RUN_TEST(test_pad_shuts_off_when_hot);

    UNITY_END();
}

void loop() {}