#include <vector>
#include <iostream>
#include "Tile.h"

class Map
{
    private:
        std::vector<std::vector<Tile>> worldMap;

    public:
        Map(int mapWidth, int mapHeight);

        std::vector<std::vector<Tile>>& GetMap();
};
