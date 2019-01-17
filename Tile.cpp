#include "Tile.h"

using namespace std;

Tile::Tile(TileType type, unsigned int color, string wallTextureFile) : type(type), color(color)
{
}

unsigned int Tile::GetColor()
{
    return this->color;
}

TileType Tile::GetType()
{
    return this->type;
}

std::string Tile::GetWallTextureFilename()
{
    return this->wallTextureFilename;
}
