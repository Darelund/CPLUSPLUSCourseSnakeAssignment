#include "map.h"

void map::SetMap(int aheight, int alength)
{
    height = aheight;
    length = alength;

    for (size_t y = 0; y < height; y++)
    {
        map.push_back(std::vector<int>());
        for (size_t x = 0; x < length; x++)
        {
            map[y].push_back(0);
        }
    }

    for (size_t y = 0; y < height; y++)
    {
        for (size_t x = 0; x < length; x++)
        {
            if (y == 1 || x == 1) {
                map[y][x] = 2;
            }
            if (y == 0 || x == 0 || x == (length - 1) || y == (height - 1)) {
                map[y][x] = 0;
            }
            else
            {
                map[y][x] = 1;
            }
        }
    }
}
void map::setTile(int y, int x, int newValue)
{
    map[y][x] = newValue;
}
int map::getTile(int y, int x)
{
    return map[y][x];
}