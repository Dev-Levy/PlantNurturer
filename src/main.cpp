#include <Arduino.h>

#include <navigationKeys/keys.h>

#include <displayManager.h>
#include <timeManager.h>
#include <sensorManager.h>
#include <actuatorManager.h>

#include <nurturerLogic.h>
#include <menuSystem.h>

#include <config.h>

static KeyPress lastKey = KeyPress::NONE;
static bool turnedOff = false;

TimeManager clock;
DisplayManager display;
SensorManager sensors;
ActuatorManager actuators;
PlantManager plant;

MenuSystem menu(clock, display, sensors, actuators, plant);
NurturerLogic logic(clock, sensors, actuators, plant);

void setup()
{
  Serial.begin(9600);

  setupPins();

  clock.begin();
  display.begin();
  sensors.begin();
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

  if (menu.selectedPlantConfig >= 0 && menu.selectedPlantConfig < PLANT_COUNT)
  {
    logic.control(menu.storedConfigs[menu.selectedPlantConfig]);
    turnedOff = false;
  }
  else
  {
    if (!turnedOff)
    {
      logic.turnOffAll();
      turnedOff = true;
    }
  }
}