#pragma once
#include <Arduino.h>

struct MenuPage;
typedef void (*MenuCallback)(void *ctx);

struct MenuItem
{
    const __FlashStringHelper *label;
    const MenuPage *targetPage;
    MenuCallback callback;
    void *callbackContext;
};

struct MenuPage
{
    const __FlashStringHelper *title;
    const MenuItem *items;
    uint8_t itemCount;
    const MenuPage *parent;
};