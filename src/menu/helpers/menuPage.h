// MenuPage.h
#pragma once
#include <Arduino.h>
#include "../../config.h"

struct MenuPage;

struct MenuItem
{
    const char *label;
    MenuPage *targetPage;
    void (*callback)();
};

struct MenuPage
{
    const char *title;
    MenuItem items[MAX_MENU_ITEMS];
    int itemCount;
    MenuPage *parent;
};