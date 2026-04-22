# Plant Nurturer - Project Details

this is how my folder structure looks like:
.gitignore  
platformio.ini  
.github\instructions\project_details.instructions.md
include\config.h  
lib\actuators\actuatorManager.cpp  
lib\actuators\actuatorManager.h  
lib\menu\menuSystem.cpp  
lib\menu\menuSystem.h  
lib\menu\callbacks\callbacks.cpp  
lib\menu\helpers\helper.cpp  
lib\menu\helpers\helper.h  
lib\menu\navigationKeys\keys.cpp  
lib\menu\navigationKeys\keys.h  
lib\menu\pages\pages.cpp  
lib\menu\pages\pages.h  
lib\menu\pages\helpers\forwardDeclarations.h  
lib\menu\pages\helpers\itemLabels.cpp  
lib\menu\pages\helpers\pageItems.cpp  
lib\menu\pages\helpers\titles.cpp  
lib\menu\structs\menuPage.h  
lib\plants\plantManager.cpp  
lib\plants\plantManager.h  
lib\plants\structs\plantConfig.h  
lib\rtc\TimeManager.cpp  
lib\rtc\TimeManager.h  
lib\sensors\sensorManager.cpp  
lib\sensors\sensorManager.h  
lib\sensors\structs\sensorReadings.h  
lib\tft\displayManager.h  
lib\tft\tftManager.cpp  
src\main.cpp  
src\control\nurturerLogic.cpp  
src\control\nurturerLogic.h

This is a plant nurturer's code.
I'm using an arduino nano with 30kb of flash and 2k of ram. The code is written in C++ and uses the Arduino framework. The code reads data from various sensors (air temperature, humidity, light, soil temperature, soil moisture) and displays it on a TFT screen. The code also includes a menu system for navigating through different pages the user can see. The code is organized into different files for better modularity and readability.
I would like to be memory efficient. Especially RAM, but also flash.
My arduino nano has 8 analog pins and 14 digital pins.

I have the following sensors connected:

- RTC module using I2C (timekeeping)
- AHT20+BMP280-M using I2C (air temperature, humidity, pressure)
- BH1750 using I2C (light)
- DS18B20 using 1-Wire (soil temperature)
- Capacitive soil moisture sensor using an analog pin (soil moisture)

I have user interface components connected to the digital pins:

- 3 buttons for menu navigation (up, down, select)
- A TFT display for showing sensor data and menu options

I have actuators connected to a relay which i control using digital pins:

- A water pump for watering the plant
- A grow light for providing additional light to the plant
- A pair of fans for improving air circulation around the plant

I would like to make a system where the user can select three plants from a predefined list (currently it's tomato, chili, mint, basil). After selecting a plant the system will load a predefined config for that plant and show it in the plant's menu. The user can edit these values and the automated controlling will use these values to control the actuators. (For example, if the soil moisture is below the water limit defined in the plant's config, the system will turn on the water pump until the soil moisture reaches a certain level.)

The control is done by a NurturerLogic class which reads the sensor data, and the actual plant config and uses that to control the actuators. The control function is called in the main loop and it will check the sensor data and the plant config to decide whether to turn on or off the actuators.

The control logic should respect the config limits, but also be affected by time contraints. (For example, the water pump should not be turned on for more than a certain amount of time (for example, 1 minute) to prevent overwatering the plant. The grow light should only be turned on during the day (for example, between 6am and 6pm) and the fans should only be turned on when the air temperature is above a certain limit defined in the plant's config.)
