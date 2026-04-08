#include "menuConfig.h"

MenuPage homePage;
MenuPage mainPage;

MenuPage plantsPage;

MenuPage plant1Page;
MenuPage plant2Page;
MenuPage plant3Page;

MenuPage sensorPage;

MenuPage actuatorPage;

MenuPage setPlantPage;

void setupMenuConfiguration()
{
    // --- Home Page ---
    homePage.title = "Welcome!";
    homePage.items[0] = {"Enter Main Menu", &mainPage};
    homePage.itemCount = 1;
    homePage.parent = nullptr;

    // --- Main Page ---
    mainPage.title = "Main Menu";
    mainPage.parent = &homePage;
    mainPage.items[0] = {"Plants", &plantsPage};
    mainPage.items[1] = {"Sensors", &sensorPage};
    mainPage.items[2] = {"Actuators", &actuatorPage};
    mainPage.items[3] = {"(back)", &homePage};
    mainPage.itemCount = 4;

    // --- Plants Page ---
    plantsPage.title = "Plants";
    plantsPage.parent = &mainPage;
    plantsPage.items[0] = {"Plant1", &setPlantPage, []()
                           { activePlantIndex = 0; }};
    plantsPage.items[1] = {"Plant2", &setPlantPage, []()
                           { activePlantIndex = 1; }};
    plantsPage.items[2] = {"Plant3", &setPlantPage, []()
                           { activePlantIndex = 2; }};
    plantsPage.items[3] = {"(back)", nullptr, nullptr};
    plantsPage.itemCount = 4;

    plant1Page.parent = &plantsPage;
    plant2Page.parent = &plantsPage;
    plant3Page.parent = &plantsPage;

    // --- Set Plant Page ---
    setPlantPage.title = "Select Type";
    setPlantPage.parent = &plantsPage;
    setPlantPage.itemCount = PLANT_TYPE_COUNT;

    for (int i = 0; i < PLANT_TYPE_COUNT; i++)
    {
        const char *namePtr = (const char *)pgm_read_word(&(plantNames[i]));

        setPlantPage.items[i] = {
            namePtr,
            nullptr,
            [namePtr]()
            {
                plantsPage.items[activePlantIndex].label = namePtr;

                if (activePlantIndex == 0)
                    plant1Page.title = namePtr;
                else if (activePlantIndex == 1)
                    plant2Page.title = namePtr;
                else if (activePlantIndex == 2)
                    plant3Page.title = namePtr;
            }};
    }

    // --- Sensors Page ---
    sensorPage.title = "Live Data";
    sensorPage.parent = &mainPage;
    sensorPage.itemCount = 5;

    // --- Actuators Page ---
    actuatorPage.title = "Actuators";
    actuatorPage.parent = &mainPage;
}