#pragma once
#include "menuPage.h"

struct PlantSlot
{
    char label[12];
    MenuPage *page;
    bool isSet;
} __attribute__((packed));