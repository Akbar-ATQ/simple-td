#ifndef TERRAIN_ROAD_HPP
#define TERRAIN_ROAD_HPP

#include "entity.hpp"
#include "grid_helper.hpp"
#include "global_data.hpp"

#include "raylib.h"

class Road : public Entity
{
public:
    Road(Vec2i gridPos) : Entity{gridPos, {0, 0}, GRID_SIZE, BLUE} {};

    void Draw()
    {
        GH::DrawRec(GetRec(), color);
    };
};

#endif