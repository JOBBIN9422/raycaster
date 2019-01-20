#include "Map.h"

using namespace std;

Map::Map(int mapWidth, int mapHeight) : worldMap(mapHeight, vector<Tile>(mapWidth, Tile(TileType::FLOOR, 0)))
{
    int wallColor = 0xFFFFFFFF;
    //add top and bottom perimeter walls
    for (int x = 0; x < mapWidth; x++)
    {
        //define walls as white tiles
        this->worldMap.at(0).at(x) = Tile(TileType::WALL, wallColor, "textures/redbrick.png");
        this->worldMap.at(mapHeight - 1).at(x) = Tile(TileType::WALL, wallColor, "textures/redbrick.png");
    }

    //add left and right perimeter walls
    for (int y = 0; y < mapHeight; y++)
    {
        this->worldMap.at(y).at(0) = Tile(TileType::WALL, wallColor, "textures/redbrick.png");
        this->worldMap.at(y).at(mapWidth - 1) = Tile(TileType::WALL, wallColor, "textures/redbrick.png");
    }

    this->worldMap.at(mapHeight / 2).at(mapWidth / 2) = Tile(TileType::WALL, 0xFF0000FF, "textures/eagle.png");
}

vector<vector<Tile>>& Map::GetMap()
{
    return this->worldMap;
}
