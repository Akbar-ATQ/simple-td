#ifndef GRID_HELPER_HPP
#define GRID_HELPER_HPP

#include "global_data.hpp"

#include "raylib.h"

// ---------- Handle grid related task/calculation ---------- //
namespace GH
{
    // Draw grid map
    void Draw();
    void DrawRec(const Rectangle rec, const Color color);
    void DrawRecInCenter(const Rectangle rec, const Color color);
    void DrawCirc(const Vec2f v, const float radius, const Color color);
    void DrawCircLines(const Vec2f v, const float radius, const Color color);

    // Turn real position into grid position.
    Vec2i GridPosition(const Vec2f v);
    Vec2i GridPosition(const Vector2 v);
    int GridPosition(const float pos);

    Vec2f RealPosition(const Vec2f v);
    float RealPosition(const float pos);

    // The result is the digit represent grid position and fraction represent local position in percentage,
    // for example: x = 1.3 mean grid[1] 30% from left side.
    Vec2f MergeGridLocalPosition(const Vec2i gridPos, const Vec2f localPos);

    // Merge grid and local position then get the real position is quite common.
    Vec2f MergeReal(const Vec2i gridPos, const Vec2f localPos);

    bool OutsideMap(const int x, const int y);
    bool OutsideMap(const Vec2i gridPos);

    bool ClickGrid(const Vec2i gridPos);

    bool CollisionRecGrid(const Rectangle rec, const Vec2i gridPos);
    bool CollisionCircleRec(const Vec2f circleCenter, const float circleRadius, const Rectangle rectangle);
    bool CollisionLineRec(Vec2f p1, Vec2f p2, Rectangle rec);
}

#endif