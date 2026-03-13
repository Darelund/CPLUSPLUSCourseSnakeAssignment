#pragma once
#include <iostream>
#include <functional>
#include "console_renderer.h"
#include "Position.h"
struct text
{
    std::string text;
    Position2 pos;
    unsigned short normalColor = FOREGROUND_RED + FOREGROUND_BLUE + FOREGROUND_GREEN;

    void draw();
};
struct button
{
    text text;
    std::function<void()> performCommand;

    void draw();
    void selecteddraw();
};
