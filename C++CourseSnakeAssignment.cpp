// SnakeGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>
#include <string>
#include <vector>

#include "console_renderer.h"


//Replace map ints with this later
enum mapPart
{
    wall,
    empty,
    head,
    body,
    food
};


struct pos
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
    pos getPos()
    {
        pos newPos;
        newPos.x = x;
        newPos.y = y;
        return newPos;
    }
};

struct part
{
    //part *body = nullptr;
    pos position;
    pos oldPosition;
};

struct food
{
    pos position;
    int mapIndex = 4;
    char symbol = '$';
};


bool isDead = false;
part* player;
//Coordinates (y, x)
//8 in height
//6 in length

const int mapHeight = 15;
const int mapLength = 22;

static int map[mapHeight][mapLength] = {
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
       {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
       {0, 2, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 0},
       {0, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 0},
       {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
       {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
       {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
       {0, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 0},
       {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
       {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
       {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
       {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 4, 1, 1, 1, 1, 1, 0},
       {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
       {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
bool collided(pos p, int type)
{
    int position = map[p.y][p.x];
    if (position == type)
    {
        return true;
    }
    return false;
}
void updateRate()
{
    std::this_thread::sleep_for(std::chrono::milliseconds(70));
    // todo: fix
    //system("cls");
    drawString(0, mapLength, "Position: " + std::to_string(player->position.x) + ", " + std::to_string(player->position.y), FOREGROUND_GREEN);
    //std::cout << "X " << player->position.x << "|Y " << player->position.y << std::endl;
    renderBuffer();
}
bool canWalk(int x, int y)
{
    int position = map[y][x];
    if (position != 0)
    {
        return true;
    }
    return false;
}

void draw(std::vector<part>& parts)
{
    //0 wall
    //1 empty
    //2 player

     //Reset old draw positions for player and set new one
    for (size_t i = 0; i < parts.size(); i++)
    {
        if (parts[i].oldPosition.isEmpty() == false)
            map[parts[i].oldPosition.y][parts[i].oldPosition.x] = 1;
        map[parts[i].position.y][parts[i].position.x] = 2;
    }


    for (size_t y = 0; y < mapHeight; y++)
    {
        for (size_t x = 0; x < mapLength; x++)
        {
            auto point = map[y][x];
            if (point == 0)
            {
                drawTile(x, y, '#', FOREGROUND_RED); //Wall
                //std::cout << "#"; //Wall
            }
            else if (point == 1)
            {
                drawTile(x, y, ' '); //Wall
                //std::cout << " "; //Empty
            }
            else if (point == 2)
            {
                drawTile(x, y, 'o', FOREGROUND_BLUE); //head
                //std::cout << "o"; //Head
            }
            else if (point == 3)
            {
                drawTile(x, y, 'o', FOREGROUND_BLUE); //body
                //std::cout << "o"; //Body
            }
            else if (point == 4)
            {
                drawTile(x, y, '$', FOREGROUND_GREEN); //food
                //std::cout << "$"; //Food
            }
        }
        //std::cout << std::endl;
    }
}
pos playerInput(std::vector<part>& parts)
{
    pos input;
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

void logic(std::vector<part>& parts, pos direction)
{
    //Check for food
    int foodType = 4;

    pos newPosition;
    newPosition.setPos((parts[0].position.x + direction.x), (parts[0].position.y + direction.y));
    if (collided(newPosition, foodType))
    {
        //Eat the food
        map[parts[0].position.y][parts[0].position.x] = 1; //Make it empty
        part lastPart = parts[0];
        for (size_t i = 0; i < parts.size(); i++)
        {
            lastPart = parts[i];
        }

        part newPart;
        newPart.position.x = lastPart.position.x - direction.x;
        newPart.position.y = lastPart.position.y - direction.y;
        newPart.oldPosition.setPos(0, 0);
        parts.push_back(newPart);
        //newPart.body = nullptr;
    }


    //Check if new position is valid
    //If it is update current position
    if (canWalk(newPosition.x, newPosition.y) == true)
    {
        for (size_t i = parts.size() - 1; i > 0; i--)
        {
            parts[i].oldPosition.x = parts[i].position.x;
            parts[i].oldPosition.y = parts[i].position.y;


            parts[i].position.x = parts[i - 1].position.x;
            parts[i].position.y = parts[i - 1].position.y;
        }
        parts[0].oldPosition.x = player->position.x;
        parts[0].oldPosition.y = player->position.y;

        parts[0].position.x += direction.x;
        parts[0].position.y += direction.y;
    }
    else
    {
        //Well if you can't move then you are headed towards a wall, and that means death
        pos newPlayerPosition;
        newPlayerPosition.x = (player->position.x + direction.x);
        newPlayerPosition.y = (player->position.y + direction.y);

        if (collided(newPlayerPosition, 0) == true)
        {
            isDead = true;
        }
    }
}



std::vector<part> init()
{
    std::vector<part> defaultParts;
    // defaultParts.resize(30);

    part playerHead;
    part body1;
    part body2;

    //playerHead.body = &body1;

    //body1.body = &body2;

    //body2.body = nullptr;

    playerHead.position.x = 1;
    playerHead.position.y = 1;
    playerHead.oldPosition.setPos(0, 0);

    body1.position.x = 1;
    body1.position.y = 2;
    body1.oldPosition.setPos(0, 0);

    body2.position.x = 1;
    body2.position.y = 3;
    body2.oldPosition.setPos(0, 0);

    defaultParts.push_back(playerHead);
    defaultParts.push_back(body1);
    defaultParts.push_back(body2);

    //The above copies the objects, not references
   /* defaultParts[0].body = &defaultParts[1];
    defaultParts[1].body = &defaultParts[2];
    defaultParts[2].body = nullptr;*/

    //layer = &allParts[0];
    return defaultParts;
}

int main()
{
    std::vector<part> allParts = init();
    allParts.reserve(100);
    player = &allParts[0];
    pos input;
    pos lastInput;
    input.setPos(0, 0);
    lastInput.setPos(0, 0);

    setupCustomConsole();
    while (true)
    {
        updateRate();


        logic(allParts, input);

        draw(allParts);

        lastInput = input.getPos();
        input = playerInput(allParts);
        if (input.isEmpty() == true)
        {
            input = lastInput.getPos();
        }
        if (isDead == true)
        {
            break;
        }
    }
    deleteCustomConsole();
    // system("cls");
    // std::cout << "YOU DIED";
}