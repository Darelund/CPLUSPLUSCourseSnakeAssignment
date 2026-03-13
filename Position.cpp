#include "Position.h"
void Position2::setPos(int newx, int newy)
{
    x = newx;
    y = newy;
}
void Position2::addPos(int x, int y)
{
    x += x;
    y += y;
}
bool Position2::isEmpty()
{
    if (x == 0 && y == 0) return true;
    return false;
}
Position2 Position2::getPos()
{
    Position2 newPos;
    newPos.x = x;
    newPos.y = y;
    return newPos;
}