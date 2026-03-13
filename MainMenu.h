#pragma once
#include <string>
#include <map>
#include <vector>
#include "UI.h"
#include "gameData.h"
#include "levelData.h"
#include "menuPanel.h"

enum menu
{
    Menu,
    Level,
    Settings,
    Score,
};

class MainMenu
{
public:
    void switchButton(char& lastKey, int& currentBtn, std::vector<button>& buttons);

    menu currentMenu = menu::Menu;
    std::map<menu, menuPanel> menus;
    char lastKey = 'W';
    int currentBtn = 0;
    bool inMenu = true;

    void ChooseLevel(gameData& gameData, int level);

    void init(gameData& gameData, std::vector<levelData> data);
    void mainMenu();
};

