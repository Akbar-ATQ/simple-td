#ifndef TERRAIN_BASE_HPP
#define TERRAIN_BASE_HPP

#include "entity.hpp"
#include "grid_helper.hpp"
#include "global_data.hpp"

#include "raylib.h"

class Base : public Entity
{
public:
    Base(Vec2i gridPos) : Entity{gridPos, {0, 0}, GRID_SIZE, RED} {};

    void Draw()
    {
        GH::DrawRec(GetRec(), color);
    };
};

#endif