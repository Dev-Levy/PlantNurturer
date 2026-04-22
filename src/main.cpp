#include <Arduino.h>

#include <control/nurturerLogic.h>

#include <displayManager.h>
#include <timeManager.h>
#include <sensorManager.h>
#include <actuatorManager.h>

#include <menuSystem.h>
#include <navigationKeys/keys.h>

#include <config.h>

TimeManager clock;
DisplayManager tft;
SensorManager sensors;
ActuatorManager actuators;
PlantManager plant;

NurturerLogic logic(sensors);

MenuSystem menu(clock, tft, sensors, actuators, plant);
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

  menu.refresh();
}