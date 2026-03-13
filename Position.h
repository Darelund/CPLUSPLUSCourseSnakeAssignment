#pragma once
struct Position2
{
    int x;
    int y;
    void setPos(int newx, int newy);
    void addPos(int x, int y);
    bool isEmpty();
    Position2 getPos();
    
};
