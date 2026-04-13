#pragma once
#include <Arduino.h>
#include <config.h>

struct MenuPage;

struct MenuItem
{
    const __FlashStringHelper *label;
    const MenuPage *targetPage;
    void (*callback)(void *ctx);
    void *callbackContext;
};

struct MenuPage
{
    const __FlashStringHelper *title;
    const MenuItem *items;
    uint8_t itemCount;
    const MenuPage *parent;
};