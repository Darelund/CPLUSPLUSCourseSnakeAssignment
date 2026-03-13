#pragma once
#include "snake.h"
#include "map.h"
#include "settingData.h"
#include "inputData.h"

enum gameState {
    mainmenu,
    Loading,
    playing,
    gameover,
    Quit
};

struct gameData
{
    bool isDead = false;
    int score = 0;
    int highscore = 0;
    //part snake;
    snake* pSnake;
    inputData inputData;
    map currentMap;
    map originalMap;
    gameState currentState;
    std::string selectedLevel;
    SettingData settingData;
    gameData();

    void initGameData();

    void reset();
   
};
