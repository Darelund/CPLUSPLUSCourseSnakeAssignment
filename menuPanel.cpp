#include "menuPanel.h"


void menuPanel::InitMenuPanelButtons(std::vector<button> aButtons)
{
    buttons = aButtons;
}
void menuPanel::InitMenuPanelTexts(std::vector<text> aTexts)
{
    texts = aTexts;
}

void menuPanel::show(int& currentBtn)
{
    for (size_t i = 0; i < texts.size(); i++)
    {
        texts[i].draw();
    }
    for (size_t i = 0; i < buttons.size(); i++)
    {
        if (currentBtn == i)
        {
            buttons[i].selecteddraw();
        }
        else
            buttons[i].draw();
    }
}
void menuPanel::useButton(int& currentBtn)
{
    if (getIfBasicKeyIsCurrentlyDown('E'))
    {
        buttons[currentBtn].performCommand();
    }
}