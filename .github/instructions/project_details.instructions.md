---
description: Describe when these instructions should be loaded by the agent based on task context
# applyTo: 'Describe when these instructions should be loaded by the agent based on task context' # when provided, instructions will automatically be added to the request context when the pattern matches an attached file
---

<!-- Tip: Use /create-instructions in chat to generate content with agent assistance -->

this is how my folder structure looks like:
.gitignore  
platformio.ini  
.github\instructions\project_details.instructions.md
include\config.h  
lib\actuators\actuatorActionsInterface.h  
lib\actuators\actuatorManager.cpp  
lib\actuators\actuatorManager.h  
lib\menu\menuSystem.cpp  
lib\menu\menuSystem.h  
lib\menu\navigationKeys\keys.cpp  
lib\menu\navigationKeys\keys.h  
lib\menu\pages\pages.cpp  
lib\menu\pages\pages.h  
lib\menu\structs\menuPage.h  
lib\menu\structs\plantSlot.h  
lib\sensors\sensorActionsInterface.h  
lib\sensors\sensorManager.cpp  
lib\sensors\sensorManager.h  
lib\sensors\sensorReadings.h  
lib\tft\tftManager.cpp  
lib\tft\tftManager.h  
src\config.cpp  
src\main.cpp

This is a plant nurturer's code.
I'm using an arduino nano with 30kb of flash and 2k of ram. The code is written in C++ and uses the Arduino framework. The code reads data from various sensors (air temperature, humidity, light, soil temperature, soil moisture) and displays it on a TFT screen. The code also includes a menu system for navigating through different pages the user can see. The code is organized into different files for better modularity and readability.
I would like to be memory efficient.
My arduino nano has 8 analog pins and 14 digital pins. I have the following sensors connected:

- DHT22 for air temperature and humidity (connected to digital pin 2)
