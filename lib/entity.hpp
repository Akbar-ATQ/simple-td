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
    float offsetX = entity->position.local.x - GRID_SIZE;
    float offsetY = entity->position.local.y - GRID_SIZE;

    if (newGrid.x > entity->position.grid.x)
        entity->position.local.x = 0 + offsetX;
    else if (newGrid.x < entity->position.grid.x)
        entity->position.local.x = GRID_SIZE + entity->position.local.x;

    if (newGrid.y > entity->position.grid.y)
        entity->position.local.y = 0 + offsetY;
    else if (newGrid.y < entity->position.grid.y)
        entity->position.local.y = GRID_SIZE + entity->position.local.y;

    entity->position.grid = newGrid;
};

#endif