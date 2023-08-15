#include "tile.hpp"

#include "raylib.h"

class Road
{
public:
    Road(Vector2 pos) : data{pos, TILE_SIZE, BLUE} {};

    EntityData data;

    void Draw() { Tile::DrawRec(data.GetRec(), data.color); };
};