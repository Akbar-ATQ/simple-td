#ifndef ENTITYDATA_HPP
#define ENTITYDATA_HPP

#include "raylib.h"

struct EntityData
{
    Vector2 position;
    float size;
    Color color;

    Rectangle GetRec()
    {
        return {position.x, position.y, size, size};
    };

    void Vec2Add(Vector2 v)
    {
        position.x += v.x;
        position.y += v.y;
    };
    void Vec2Sub(Vector2 v)
    {
        position.x -= v.x;
        position.y -= v.y;
    };
    bool PositionEqual(Vector2 v)
    {
        return (position.x == v.x) && (position.y == v.y);
    };
};

#include <iostream>

struct MovingEntityData : public EntityData
{
    MovingEntityData(Vector2 pos, float size, Color color) : EntityData{pos, size, color}
    {
        gridPosition = {static_cast<int>(pos.x), static_cast<int>(pos.y)};
        localPosition = {(pos.x - static_cast<int>(pos.x)), (pos.y - static_cast<int>(pos.y))};
        // std::cout << "gridX: " << grid.x << " localX: " << localPos.x << std::endl;
    };

    Vector2 gridPosition;
    Vector2 localPosition; // Position inside grid.

    Rectangle GetRec()
    {
        float x = gridPosition.x + (((localPosition.x / TILE_SIZE) * 100) / 100);
        float y = gridPosition.y + (((localPosition.y / TILE_SIZE) * 100) / 100);
        return {x, y, size, size};
    };

    Vector2 GetRealPos()
    {
        float x = gridPosition.x + (localPosition.x / 100);
        float y = gridPosition.y + (localPosition.y / 100);

        return {x, y};
    };

    void MoveGrid(const Vector2 newGrid)
    {
        // std::cout << "nX: " << newGrid.x << " gX: " << gridPosition.x << std::endl;
        // std::cout << "nY: " << newGrid.y << " gY: " << gridPosition.y << std::endl;
        if (newGrid.x > gridPosition.x)
            localPosition.x = 0;
        else if (newGrid.x < gridPosition.x)
            localPosition.x = TILE_SIZE;

        if (newGrid.y > gridPosition.y)
            localPosition.y = 0;
        else if (newGrid.y < gridPosition.y)
            localPosition.y = TILE_SIZE;

        gridPosition = newGrid;
    };
};

#endif