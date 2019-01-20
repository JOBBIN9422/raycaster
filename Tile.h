#include <string>
#include "Enums.h"

class Tile
{
    private:
        unsigned int color; //wall color in ARGB format
        TileType type;
        std::string wallTextureFilename;

    public:
        Tile(TileType type, unsigned int color, std::string wallTextureFile = "");

        unsigned int GetColor();
        TileType GetType();

        std::string GetWallTextureFilename();
};
