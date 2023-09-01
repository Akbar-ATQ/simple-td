#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "global_data.hpp"
#include "grid_helper.hpp"

#include "raylib.h"

struct Entity
{
    Position position;
    float size;
    Color color;

    Rectangle GetRec()
    {
        Vec2f recPos = GH::MergeReal(position.grid, position.local);
        return {recPos.x, recPos.y, size, size};
    };
};

inline void MoveGrid(Entity *entity, const Vec2i &newGrid)
{
    float fractionX = (entity->position.local.x - static_cast<int>(entity->position.local.x));
    float fractionY = (entity->position.local.y - static_cast<int>(entity->position.local.y));

    if (newGrid.x > entity->position.grid.x)
        entity->position.local.x = 0 + fractionX;
    else if (newGrid.x < entity->position.grid.x)
        entity->position.local.x = GRID_SIZE + fractionX;

    if (newGrid.y > entity->position.grid.y)
        entity->position.local.y = 0 + fractionY;
    else if (newGrid.y < entity->position.grid.y)
        entity->position.local.y = GRID_SIZE + fractionY;

    entity->position.grid = newGrid;
};

#endif