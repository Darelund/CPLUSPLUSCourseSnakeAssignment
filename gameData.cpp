#include "gameData.h"


gameData::gameData()
    :
    inputData(),
    currentMap(),
    selectedLevel()

{
}

void gameData::initGameData()
{
    inputData.currentDir.setPos(0, 0);
    inputData.lastDir.setPos(0, 0);

    pSnake = new snake();
    pSnake->currentPos.setPos(1, 1);
    currentState = mainmenu;
    isDead = false;
    score = 0;
}
void gameData::reset()
{
    initGameData();
}