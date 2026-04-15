#include <Arduino.h>

#include <tftManager.h>
#include <timeManager.h>
#include <sensorManager.h>
#include <actuatorManager.h>

#include <menuSystem.h>
#include <navigationKeys/keys.h>

#include <config.h>

TimeManager clock;
TFTManager tft;
SensorManager sensors;
ActuatorManager actuators;

MenuSystem menu(clock, tft, sensors, actuators);
static unsigned long lastUpdate = 0;
static KeyPress lastKey = KeyPress::NONE;

void setup()
{
  Serial.begin(9600);

  setupPins();
  clock.begin();
  tft.begin();
  menu.begin();
}

void loop()
{
  KeyPress key = readKeys();

  if (key != KeyPress::NONE && lastKey == KeyPress::NONE)
  {
    menu.processKey(key);
    menu.draw();
  }

  lastKey = key;

  if (millis() - lastUpdate > 2000)
  {
    menu.updateSensorValues();
    clock.updateTime();
    lastUpdate = millis();
  }
}