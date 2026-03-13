#pragma once
#include "Position.h"
struct snake
{
    ~snake();
    Position2 currentPos;
    snake* nextPart = nullptr;
    void NextPos(Position2 pos);
};
