// SnakeGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <string>
#include <vector>
#include <time.h>

#include "console_renderer.h"

enum gameSate {
    playing,
    gameover,

};
//Replace map ints with this later
enum mapPart
{
    wall,
    empty,
    head,
    body,
    food
};

struct map
{
    int height;
    int length;
    int *mapPointer;

    void SetMap(int aheight, int alength, int *p)
    {
        height = aheight;
        length = alength;
        mapPointer = p;
    }
    void setTile(int x, int y, int newValue)
    {
        mapPointer[x * length + y] = newValue;
    }
    int getTile(int x, int y)
    {
        return mapPointer[x * length + y];
    }
};

struct position
{
    int x;
    int y;
    void setPos(int newx, int newy)
    {
        x = newx;
        y = newy;
    }
    bool isEmpty()
    {
        if (x == 0 && y == 0) return true;
        return false;
    }
    position getPos()
    {
        position newPos;
        newPos.x = x;
        newPos.y = y;
        return newPos;
    }
};

struct part
{
    position currrentPosition;
    position oldPosition;
};

struct snakeFood
{
    position position;
    int mapIndex = 4;
    char symbol = '$';
};




std::vector<part> init()
{
    std::vector<part> defaultParts;

    part playerHead;
    part body1;
    part body2;

    playerHead.currrentPosition.x = 1;
    playerHead.currrentPosition.y = 1;
    playerHead.oldPosition.setPos(0, 0);

    body1.currrentPosition.x = 1;
    body1.currrentPosition.y = 2;
    body1.oldPosition.setPos(0, 0);

    body2.currrentPosition.x = 1;
    body2.currrentPosition.y = 3;
    body2.oldPosition.setPos(0, 0);

    defaultParts.push_back(playerHead);
    defaultParts.push_back(body1);
    defaultParts.push_back(body2);
    defaultParts.reserve(100);
    return defaultParts;
}


//part* player;
//Coordinates (y, x)
//8 in height
//6 in length
//const int currentMapHeight;
//const int currentMapLength;

struct inputData
{
    position currentDirection;
    position lastDirection;
};
struct gameData
{
    bool isDead = false;
    int score = 0;
    //snakeData snakeData;
    std::vector<part> snakeParts;
    part* head;
    inputData inputData;
    map currentMap;
    map originalMap;

    gameSate currentState;

    gameData()
        : snakeParts(),
        head(),
        inputData(),
        currentMap()
    {
    }

    void initGameData()
    {
        inputData.currentDirection.setPos(0, 0);
        inputData.lastDirection.setPos(0, 0);

        snakeParts = init();
        head = &snakeParts[0];

        currentState = playing;
        isDead = false;
    }
    void reset()
    {
        initGameData();
    }
};


bool collided(position p, int type, map &map)
{
    int position = map.getTile(p.y, p.x);
    if (position == type)
    {
        return true;
    }
    return false;
}
void updateRate(gameData &gameData)
{
    std::this_thread::sleep_for(std::chrono::milliseconds(70));
    // todo: fix
    //system("cls");
    drawString(0, gameData.currentMap.length + 2, "Score: " + std::to_string(gameData.score), FOREGROUND_GREEN);
    drawString(0, gameData.currentMap.length, "Current Position: " + std::to_string(gameData.head->currrentPosition.x) + ", " + std::to_string(gameData.head->currrentPosition.y), FOREGROUND_GREEN);
    drawString(0, gameData.currentMap.length + 1, "currenDirection: " + std::to_string(gameData.inputData.currentDirection.x) + ", " + std::to_string(gameData.inputData.currentDirection.y), FOREGROUND_GREEN);

    renderBuffer();
}
bool canWalk(int x, int y, map& map)
{
    int position = map.getTile(y, x);
    if (position != 0)
    {
        return true;
    }
    return false;
}

void draw(gameData& gameData)
{
    //0 wall
    //1 empty
    //2 player

     //Reset old draw positions for player and set new one
    for (size_t i = 0; i < gameData.snakeParts.size(); i++)
    {
        if (gameData.snakeParts[i].oldPosition.isEmpty() == false)
            gameData.currentMap.setTile(gameData.snakeParts[i].oldPosition.y, gameData.snakeParts[i].oldPosition.x, 1);

        gameData.currentMap.setTile(gameData.snakeParts[i].currrentPosition.y, gameData.snakeParts[i].currrentPosition.x, 2);
      /*  map[parts[i].position.y][parts[i].position.x] = 2;*/
    }

    for (size_t y = 0; y < gameData.currentMap.height; y++)
    {
        for (size_t x = 0; x < gameData.currentMap.length; x++)
        {
            auto point = gameData.currentMap.getTile(y, x);
            if (point == 0)
            {
                drawTile(x, y, '#', FOREGROUND_RED); //Wall
            }
            else if (point == 1)
            {
                drawTile(x, y, ' '); //Wall
            }
            else if (point == 2)
            {
                drawTile(x, y, 'o', FOREGROUND_BLUE); //head
            }
            else if (point == 3)
            {
                drawTile(x, y, 'o', FOREGROUND_BLUE); //body
            }
            else if (point == 4)
            {
                drawTile(x, y, '$', FOREGROUND_GREEN); //food
            }
        }
    }
}
position playerInput()
{
    position input;
    input.x = 0;
    input.y = 0;

    //Give us those sweet keys
    char key;
    if (_kbhit() == true)
    {
        key = _getch();

        if (key != '\0')
        {
            if (key == 'w')
                input.y = -1;
            else if (key == 's')
                input.y = 1;
            else
                input.y = 0;


            if (key == 'a')
                input.x = -1;
            else if (key == 'd')
                input.x = 1;
            else
                input.x = 0;

        }
    }
    return input;
}
void generateFood(gameData& gameData)
{
    //snakeFood someFood;

   //someFood.position.setPos((rand() % (gameData.currentMap.height - 2)) + 1, (rand() % (gameData.currentMap.length - 2)) + 1);
    auto x = (rand() % (gameData.currentMap.height - 2)) + 1;
    auto y = (rand() % (gameData.currentMap.length - 2)) + 1;
    gameData.currentMap.setTile(x, y, 4);
}
void generateWall(gameData& gameData)
{
    while (true)
    {
        auto x = (rand() % (gameData.currentMap.height - 2)) + 1;
        auto y = (rand() % (gameData.currentMap.length - 2)) + 1;
        if (gameData.currentMap.getTile(x, y) == 1)
        {
            gameData.currentMap.setTile(x, y, 0);
            break;
        }
    }
}
void logic(gameData &gameData)
{
   // std::vector<part>& parts, position direction, map& map


    //Check for food
    int foodType = 4;

    position newPosition;
    newPosition.setPos((gameData.head->currrentPosition.x + gameData.inputData.currentDirection.x), (gameData.head->currrentPosition.y + gameData.inputData.currentDirection.y));
    if (collided(newPosition, foodType, gameData.currentMap))
    {
        gameData.score += 10;
        //Eat the food
        gameData.currentMap.setTile(gameData.head->currrentPosition.y,gameData.head->currrentPosition.x, 1); //Make it empty
        part* lastPart = gameData.head;
        for (size_t i = 0; i < gameData.snakeParts.size(); i++)
        {
            lastPart = &gameData.snakeParts[i];
        }

        part newPart;
        newPart.currrentPosition.x = lastPart->currrentPosition.x - gameData.inputData.currentDirection.x;
        newPart.currrentPosition.y = lastPart->currrentPosition.y - gameData.inputData.currentDirection.y;
        newPart.oldPosition.setPos(0, 0);
        gameData.snakeParts.push_back(newPart);
        generateFood(gameData);
        generateWall(gameData);
    }


    //Check if new position is valid
    //If it is update current position
    if (canWalk(newPosition.x, newPosition.y, gameData.currentMap) == true)
    {
        for (size_t i = gameData.snakeParts.size() - 1; i > 0; i--)
        {
            gameData.snakeParts[i].oldPosition.x = gameData.snakeParts[i].currrentPosition.x;
            gameData.snakeParts[i].oldPosition.y = gameData.snakeParts[i].currrentPosition.y;


            gameData.snakeParts[i].currrentPosition.x = gameData.snakeParts[i - 1].currrentPosition.x;
            gameData.snakeParts[i].currrentPosition.y = gameData.snakeParts[i - 1].currrentPosition.y;
        }
        gameData.snakeParts[0].oldPosition.x = gameData.head->currrentPosition.x;
        gameData.snakeParts[0].oldPosition.y = gameData.head->currrentPosition.y;

        gameData.snakeParts[0].currrentPosition.x += gameData.inputData.currentDirection.x;
        gameData.snakeParts[0].currrentPosition.y += gameData.inputData.currentDirection.y;
    }
    else
    {
        //Well if you can't move then you are headed towards a wall, and that means death
        position newPlayerPosition;
        newPlayerPosition.x = (gameData.head->currrentPosition.x + gameData.inputData.currentDirection.x);
        newPlayerPosition.y = (gameData.head->currrentPosition.y + gameData.inputData.currentDirection.y);

        if (collided(newPlayerPosition, 0, gameData.currentMap) == true)
        {
            gameData.isDead = true;
        }
    }
}


int main(int argc, char* argv[])
{
    std::srand(static_cast<unsigned int > (std::time(nullptr)));
   bool isRunning = true;

   const int mapHeight1 = 10;
   const int mapLength1 = 22;

   const int mapHeight2 = 15;
   const int mapLength2 = 22;

   const int mapHeight3 = 25;
   const int mapLength3 = 25;


   int map1[mapHeight1][mapLength1] = {
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 2, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
   };

   int map2[mapHeight2][mapLength2] = {
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
          {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
   };

   int map3[mapHeight3][mapLength3] = {
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0},
          {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,0, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,1, 1,0},
          {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0,0}
   };
    gameData gameData;
    //map map;
    gameData.initGameData();
    std::string l = "Level1";

    if (l == "Level1")
    {
        gameData.currentMap.SetMap(mapHeight1, mapLength1, (&map1[0][0]));
    }
    else if (l == "Level2")
    {
        gameData.currentMap.SetMap(mapHeight2, mapLength2, (&map2[0][0]));
    }
    else if (l == "Level3")
    {
        gameData.currentMap.SetMap(mapHeight3, mapLength3, (&map3[0][0]));
    }
    setupCustomConsole();


    //Spawn food
    //Mechanic
    //Snake shouldn't be able to go into itself
    //Reset maps


    while (isRunning)
    {
        switch (gameData.currentState) {
        case playing:
           

            updateRate(gameData);


            logic(gameData);

            draw(gameData);

            gameData.inputData.lastDirection = gameData.inputData.currentDirection.getPos();
            gameData.inputData.currentDirection = playerInput();

            if (gameData.inputData.currentDirection.isEmpty() == true)
            {
                gameData.inputData.currentDirection = gameData.inputData.lastDirection.getPos();
            }
            if (gameData.isDead == true)
            {
                gameData.currentState = gameover;
                clearBuffer();
            }
            break;

        case gameover:
           
            drawString(0, 0, "YOU DIED");
            drawString(0, 1, "Press x to quit");
           
            //std::this_thread::sleep_for(std::chrono::milliseconds(4000));
            char key;
            if (_kbhit() == true)
            {
                key = _getch();
                if (key != '\0')
                {
                   /* if (key == 'r')
                    {
                        gameData.reset();
                        clearBuffer();
                    }*/
                    if (key == 'x')
                    {
                        isRunning = false;
                        break;
                    }
                }
            }
            renderBuffer();
            // system("cls");
            // std::cout << "YOU DIED";
            break;
        }
    }
    deleteCustomConsole();
    
}