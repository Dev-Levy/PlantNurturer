#include <Arduino.h>
#include "tft/tftManager.h"
#include "menu/menuSystem.h"
#include "menu/navigationKeys/keys.h"
#include "sensors/sensorManager.h"
#include "actuators/actuatorManager.h"

#include "config.h"

ActuatorManager actuators;
SensorManager sensors;
TFTManager tft;
MenuSystem menu(tft, sensors, actuators);
static unsigned long lastSensorRead = 0;

void setup()
{
  setupPins();
  tft.begin();
  menu.begin();

  Serial.begin(9600);
}

void loop()
{
  if (millis() - lastSensorRead > 200)
  {
    menu.updateSensorValues();
    lastSensorRead = millis();
  }
  menu.updateSensorValues();

  int key = readKeys();
  if (key)
  {
    menu.processKey(key);
    menu.draw();
    delay(100);
  }
}