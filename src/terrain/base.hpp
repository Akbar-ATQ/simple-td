#ifndef BASE_HPP
#define BASE_HPP

#include "tile.hpp"

#include "raylib.h"

class Base
{
public:
    Base(Vector2 pos) : data{pos, TILE_SIZE, RED} {};

    EntityData data;

    void Draw() { Tile::DrawRec(data.GetRec(), data.color); };
};

#endif