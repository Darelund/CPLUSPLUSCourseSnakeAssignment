#pragma once
#include <string>
#include <vector>
#include "UI.h"

struct menuPanel
{
    std::vector<button> buttons;
    std::vector<text> texts;

    void InitMenuPanelButtons(std::vector<button> aButtons);
    void InitMenuPanelTexts(std::vector<text> aTexts);
    void show(int& currentBtn);
    void useButton(int& currentBtn);

};
