#pragma once
#include <vector>

//TODO: Replace map ints with this later
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
    std::vector<std::vector<int>> map;
    void SetMap(int aheight, int alength);
    void setTile(int y, int x, int newValue);
    int getTile(int y, int x);
};