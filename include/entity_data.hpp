#ifndef ENTITYDATA_HPP
#define ENTITYDATA_HPP

#include "raylib.h"

struct EntityData
{
    Vector2 position;
    float size;
    Color color;

    Rectangle GetRec() { return {position.x, position.y, size, size}; };

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
    bool PositionEqual(Vector2 v) { return (position.x == v.x) && (position.y == v.y); };
};

#endif