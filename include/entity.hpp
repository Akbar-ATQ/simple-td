#ifndef ENTITY_HPP
#define ENTITY_HPP

#include "global_data.hpp"
#include "grid_helper.hpp"

#include "raylib.h"

struct Entity
{
    Vec2i gridPosition;
    Vec2f localPosition; // Position inside grid.
    float size;
    Color color;

    Rectangle GetRec()
    {
        Vec2f recPos = GH::RealPosition(GH::MergeGridLocalPosition(gridPosition, localPosition));
        return {recPos.x, recPos.y, size, size};
    };
};

inline void MoveGrid(Entity *entity, const Vec2i &newGrid)
{
    float fractionX = (entity->localPosition.x - static_cast<int>(entity->localPosition.x));
    float fractionY = (entity->localPosition.y - static_cast<int>(entity->localPosition.y));

    if (newGrid.x > entity->gridPosition.x)
        entity->localPosition.x = 0 + fractionX;
    else if (newGrid.x < entity->gridPosition.x)
        entity->localPosition.x = GRID_SIZE + fractionX;

    if (newGrid.y > entity->gridPosition.y)
        entity->localPosition.y = 0 + fractionY;
    else if (newGrid.y < entity->gridPosition.y)
        entity->localPosition.y = GRID_SIZE + fractionY;

    entity->gridPosition = newGrid;
};

#endif