#include <Arduino.h>

#include <tftManager.h>
#include <sensorManager.h>
#include <actuatorManager.h>

#include <menuSystem.h>
#include <navigationKeys/keys.h>

#include <config.h>

ActuatorManager actuators;
SensorManager sensors;
TFTManager tft;
MenuSystem menu(tft, sensors, actuators);
static unsigned long lastSensorRead = 0;

void setup()
{
  Serial.begin(9600);

  setupPins();
  tft.begin();
  menu.begin();
}

void loop()
{
  if (millis() - lastSensorRead > 200)
  {
    menu.updateSensorValues();
    lastSensorRead = millis();
  }

  KeyPress key = readKeys();
  if (key != KeyPress::NONE)
  {
    menu.processKey(key);
    menu.draw();
    delay(100);
  }
}