#include <unity.h>
#include <Arduino.h>
#include "config.h"
#include "actuatorManager.h"

ActuatorManager actuatorManager;

void setUp()
{
    actuatorManager.resetStates();
}

void test_togglePump_changes_state_pumpOn()
{
    actuatorManager.togglePump();
    TEST_ASSERT_TRUE(actuatorManager.state.pumpOn);
    TEST_ASSERT_TRUE(actuatorManager.pumpWaitTime > 0);
    TEST_ASSERT_TRUE(digitalRead(PUMP) == LOW);
    actuatorManager.togglePump();
    TEST_ASSERT_FALSE(actuatorManager.state.pumpOn);
    TEST_ASSERT_TRUE(digitalRead(PUMP) == HIGH);
}
void test_toggleLight_changes_state_lightOn()
{
    actuatorManager.toggleLight();
    TEST_ASSERT_TRUE(actuatorManager.state.lightOn);
    TEST_ASSERT_TRUE(actuatorManager.lightWaitTime > 0);
    TEST_ASSERT_TRUE(digitalRead(LIGHT) == LOW);
    actuatorManager.toggleLight();
    TEST_ASSERT_FALSE(actuatorManager.state.lightOn);
    TEST_ASSERT_TRUE(digitalRead(LIGHT) == HIGH);
}
void test_toggleFan_changes_state_fanOn()
{
    actuatorManager.toggleFan();
    TEST_ASSERT_TRUE(actuatorManager.state.fanOn);
    TEST_ASSERT_TRUE(actuatorManager.fanWaitTime > 0);
    TEST_ASSERT_TRUE(digitalRead(FAN) == LOW);
    actuatorManager.toggleFan();
    TEST_ASSERT_FALSE(actuatorManager.state.fanOn);
    TEST_ASSERT_TRUE(digitalRead(FAN) == HIGH);
}
void test_togglePad_changes_state_padOn()
{
    actuatorManager.togglePad();
    TEST_ASSERT_TRUE(actuatorManager.state.padOn);
    TEST_ASSERT_TRUE(actuatorManager.padWaitTime > 0);
    TEST_ASSERT_TRUE(digitalRead(PAD) == LOW);
    actuatorManager.togglePad();
    TEST_ASSERT_FALSE(actuatorManager.state.padOn);
    TEST_ASSERT_TRUE(digitalRead(PAD) == HIGH);
}

void setup()
{
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_togglePump_changes_state_pumpOn);
    RUN_TEST(test_toggleLight_changes_state_lightOn);
    RUN_TEST(test_toggleFan_changes_state_fanOn);
    RUN_TEST(test_togglePad_changes_state_padOn);

    UNITY_END();
}

void loop()
{
}
