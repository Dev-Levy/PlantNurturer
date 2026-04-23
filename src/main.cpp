#include <Arduino.h>

#include <control/nurturerLogic.h>

#include <displayManager.h>
#include <timeManager.h>
#include <sensorManager.h>
#include <actuatorManager.h>

#include <menuSystem.h>
#include <navigationKeys/keys.h>

#include <config.h>

static KeyPress lastKey = KeyPress::NONE;
static int8_t lastSelectedPlantIndex = -1;
static PlantConfig activeConfig{};

TimeManager clock;
DisplayManager tft;
SensorManager sensors;
ActuatorManager actuators;
PlantManager plant;

MenuSystem menu(clock, tft, sensors, actuators, plant);
NurturerLogic logic(clock, sensors, actuators, plant);

void setup()
{
  Serial.begin(9600);

  setupPins();

  clock.begin();
  tft.begin();
  sensors.begin();
  menu.begin();

  if (lastSelectedPlantIndex != menu.selectedPlantConfig)
  {
    if (plant.getPlantConfig(menu.selectedPlantConfig, activeConfig))
    {
      lastSelectedPlantIndex = menu.selectedPlantConfig;
      Serial.print(F("Active plant config set to: "));
      Serial.println(menu.selectedPlantConfig);
    }
  }
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

  if (menu.selectedPlantConfig != lastSelectedPlantIndex)
  {
    if (plant.getPlantConfig(menu.selectedPlantConfig, activeConfig))
    {
      lastSelectedPlantIndex = menu.selectedPlantConfig;
      Serial.print(F("Active plant config changed: "));
      Serial.println(menu.selectedPlantConfig);
    }
  }

  if (lastSelectedPlantIndex >= 0)
  {
    logic.control(activeConfig);
  }
}