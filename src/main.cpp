#include <Arduino.h>

#include <navigationKeys/keys.h>

#include <displayManager.h>
#include <timeManager.h>
#include <sensorManager.h>
#include <actuatorManager.h>

#include <nurturerLogic.h>
#include <menuSystem.h>

#include <config.h>

TimeManager clock;
SensorManager sensors;
ActuatorManager actuators;
PlantManager plant;
PlantConfig config;

NurturerLogic logic(clock, sensors, actuators, plant);

void setup()
{
  Serial.begin(9600);
  delay(2000);

  setupPins();

  clock.begin();
  sensors.begin();

  plant.getPlantConfig(CHILI_CONFIG_ID, config);
  // serial out the config value for testing
  Serial.println(F("Config idx, SunnyHours, WaterLimit, WaterMl, IdealTemp, IdealSoilTemp"));
  Serial.print(config.idx);
  Serial.print(F(", "));
  Serial.print(config.sunnyHours);
  Serial.print(F(", "));
  Serial.print(config.waterLimit);
  Serial.print(F(", "));
  Serial.print(config.waterMl);
  Serial.print(F(", "));
  Serial.print(config.idealTemp);
  Serial.print(F(", "));
  Serial.println(config.idealSoilTemp);

  Serial.print(F("Time, Light, Temp, Humidity, SoilTemp, SoilMoist, Pump, Light, Fan, Pad, "));
  Serial.print(F("ShouldWater, WateredEnough, CanTogglePump, "));
  Serial.print(F("IsThereEnoughLight, ShouldHaveLight, CanToggleLight, "));
  Serial.print(F("IsTooHot, IsTooCold, IsTooHumid, IsHumidityIdeal, CanToggleFan, "));
  Serial.print(F("IsSoilTooHot, IsSoilTooCold, CanTogglePad"));
  Serial.println();
}

void loop()
{
  logic.control(config);
}