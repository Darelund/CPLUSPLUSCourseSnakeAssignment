#include "snake.h"

snake::~snake()
{
    if (nextPart != nullptr)
    {
        delete nextPart;
    }
}

void snake::NextPos(Position2 pos)
{
    if (nextPart != nullptr)
    {
        nextPart->NextPos(currentPos);
    }
    currentPos = pos;
}