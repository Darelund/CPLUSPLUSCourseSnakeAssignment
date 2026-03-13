#include "Game.h"


bool Game::collided(Position2 p, int type, map& map)
{
    int position = map.getTile(p.y, p.x);
    if (position == type)
    {
        return true;
    }
    return false;
}
bool Game::canWalk(int y, int x, map& map)
{
    int position = map.getTile(y, x);
    if (position != 0)
    {
        return true;
    }
    return false;
}


void Game::updateRate(gameData& gameData)
{
    renderBuffer();
    std::this_thread::sleep_for(std::chrono::milliseconds(70));
}

void Game::DrawUI(gameData& gameData)
{
    drawString(0, gameData.currentMap.length + 0, "Score: " + std::to_string(gameData.score), FOREGROUND_GREEN);
    drawString(0, gameData.currentMap.length + 1, "HighScore: " + std::to_string(gameData.highscore), FOREGROUND_GREEN);

    /* drawString(0, gameData.currentMap.length + 3, "currenDirection: " + std::to_string(gameData.inputData.currentDir.x) + ", " + std::to_string(gameData.inputData.currentDir.y), FOREGROUND_GREEN);
     int n = 5;
     auto* lastPart = gameData.pSnake;
     while (lastPart != nullptr)
     {
         drawString(0, gameData.currentMap.length + n, "Current Position: " + std::to_string(lastPart->currentPos.x) + ", " + std::to_string(lastPart->currentPos.y), FOREGROUND_GREEN);
         lastPart = lastPart->nextPart;
         n++;
     }*/
}
void Game::drawWorld(gameData& gameData)
{
    for (size_t y = 0; y < gameData.currentMap.height; y++)
    {
        for (size_t x = 0; x < gameData.currentMap.length; x++)
        {
            auto point = gameData.currentMap.getTile(y, x);
            if (point == 0)
            {
                drawTile(x, y, '#', gameData.settingData.wallColor); //Wall
            }
            else if (point == 1)
            {
                drawTile(x, y, ' '); //empty
            }
            else if (point == 2)
            {
                drawTile(x, y, 'o', gameData.settingData.snakeColor); //snake
            }
            else if (point == 4)
            {
                drawTile(x, y, '$', gameData.settingData.foodColor); //food
            }
        }
    }
}

void Game::draw(gameData& gameData)
{
    DrawUI(gameData);
    drawWorld(gameData);
}


Position2 Game::playerInput()
{
    Position2 input;
    input.x = 0;
    input.y = 0;

    if (getIfBasicKeyIsCurrentlyDown('W'))
        input.y = -1;
    else if (getIfBasicKeyIsCurrentlyDown('S'))
        input.y = 1;
    else
        input.y = 0;


    if (getIfBasicKeyIsCurrentlyDown('A'))
        input.x = -1;
    else if (getIfBasicKeyIsCurrentlyDown('D'))
        input.x = 1;
    else
        input.x = 0;

    return input;
}

void Game::updateRandomTile(gameData& gameData, int type)
{
    while (true)
    {
        auto x = (rand() % (gameData.currentMap.height - 2)) + 1;
        auto y = (rand() % (gameData.currentMap.length - 2)) + 1;
        if (gameData.currentMap.getTile(x, y) == 1)
        {
            gameData.currentMap.setTile(x, y, type);
            break;
        }
    }
}

void Game::logic(gameData& gameData)
{
    //Check for food
    int foodType = 4;

    Position2 newPosition;
    newPosition.setPos((gameData.pSnake->currentPos.x + gameData.inputData.currentDir.x), (gameData.pSnake->currentPos.y + gameData.inputData.currentDir.y));

    if (collided(newPosition, foodType, gameData.currentMap))
    {
        gameData.score += 10;
        gameData.currentMap.setTile(gameData.pSnake->currentPos.y, gameData.pSnake->currentPos.x, 1); //Make it empty

        auto* lastPart = gameData.pSnake;
        while (lastPart->nextPart != nullptr)
        {
            lastPart = lastPart->nextPart;
        }

        snake* newPart = new snake();
        lastPart->nextPart = newPart;
        newPart->currentPos.setPos(lastPart->currentPos.x - gameData.inputData.currentDir.x, lastPart->currentPos.y - gameData.inputData.currentDir.y);


        updateRandomTile(gameData, 4);
        updateRandomTile(gameData, 0);
    }

    //Check if new position is valid
    //If it is update current position
    if (canWalk(newPosition.y, newPosition.x, gameData.currentMap) == true)
    {

        snake* currentSnake = gameData.pSnake;
        while (currentSnake != nullptr)
        {
            gameData.currentMap.setTile(currentSnake->currentPos.y, currentSnake->currentPos.x, 1);
            currentSnake = currentSnake->nextPart;
        }

        gameData.pSnake->NextPos(newPosition);

        currentSnake = gameData.pSnake;
        while (currentSnake != nullptr)
        {

            gameData.currentMap.setTile(currentSnake->currentPos.y, currentSnake->currentPos.x, 2);

            currentSnake = currentSnake->nextPart;
        }

        gameData.pSnake->currentPos.addPos(gameData.inputData.currentDir.x, gameData.inputData.currentDir.y);
    }
    else
    {
        //Well if you can't move then you are headed towards a wall, and that means death
        Position2 newPlayerPosition;
        newPlayerPosition.x = (gameData.pSnake->currentPos.x + gameData.inputData.currentDir.x);
        newPlayerPosition.y = (gameData.pSnake->currentPos.y + gameData.inputData.currentDir.y);

        if (collided(newPlayerPosition, 0, gameData.currentMap) == true)
        {
            gameData.isDead = true;
            delete gameData.pSnake; //FIX
        }
    }
}

void Game::run()
{
    MainMenu mainMenu;
    gameData gameData;
    SavingSystem savingSystem;
    std::vector<levelData> data;
    SavingSettings savingSettings;
    SettingData settingData;

    //Load again once each time
    gameData.initGameData();
    data = savingSystem.load();
    settingData = savingSettings.load();
    gameData.settingData = settingData;
    mainMenu.init(gameData, data);

    while (isGameRunning)
    {
        switch (gameData.currentState) {
        case mainmenu:
            mainMenu.mainMenu();
            if (gameData.currentState != gameState::Quit)
                gameData.currentState = gameState::Loading;
            break;
        case Loading:
            settingData = savingSettings.load();
            gameData.settingData = settingData;
            //Get highscore and initilize map data
            for (size_t i = 0; i < data.size(); i++)
            {
                if (data[i].level == gameData.selectedLevel)
                {
                    gameData.highscore = data[i].highScore;
                    gameData.currentMap.SetMap(data[i].height, data[i].length);
                    break;
                }
            }
            //Set player and snake position
            gameData.currentMap.setTile(1, 1, 2);
            gameData.currentMap.setTile(5, 5, 4);

            gameData.currentState = gameState::playing;
            break;
        case playing:
            updateRate(gameData);
            logic(gameData);
            draw(gameData);

            gameData.inputData.lastDir = gameData.inputData.currentDir.getPos();
            gameData.inputData.currentDir = playerInput();

            if (gameData.inputData.currentDir.isEmpty() == true)
            {
                gameData.inputData.currentDir = gameData.inputData.lastDir.getPos();
            }
            if (gameData.isDead == true)
            {
                gameData.currentState = gameState::gameover;
                clearBuffer();
            }
            break;

        case gameover:

            drawString(0, 0, "YOU DIED");
            drawString(0, 1, "Press x to quit || c return Main Menu");
            for (size_t i = 0; i < data.size(); i++)
            {
                if (data[i].level == gameData.selectedLevel)
                {
                    if (gameData.score > gameData.highscore)
                    {
                        data[i].highScore = gameData.score;
                        savingSystem.save(data);
                        break;
                    }
                }
            }
            if (getIfBasicKeyIsCurrentlyDown('X'))
            {
                isGameRunning = false;
                break;
            }
            if (getIfBasicKeyIsCurrentlyDown('C'))
            {
                gameData.currentState = gameState::mainmenu;
                gameData.reset();
                break;
            }
            renderBuffer();
            break;
        case Quit:
            isGameRunning = false;
            break;
        }
    }
}