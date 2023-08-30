#ifndef TILE_HPP
#define TILE_HPP

#include "raylib.h"

#include <string>

// ---------- Handle tile related task/calculation ---------- //
// [TODO] refactor tile into grid
// [Todo]
// 3. rename tile.hpp to grid_helper.hpp
// move it to grid/
namespace Tile
{
    void Draw(); // Draw the map in tile grid
    void DrawRec(const Rectangle rec, const Color color);
    void DrawRecOnCenter(const Rectangle rec, const Color color);
    void DrawCirc(const Vector2 v, const float radius, const Color color);
    void DrawCircLines(const Vector2 v, const float radius, const Color color);

    Vector2 Position(const Vector2 v);
    Vector2 RealPosition(const Vector2 v);
    float RealPosition(const float pos);

    bool OutsideMap(const int x, const int y);

    bool ClickTile(const Vector2 tilePos);
    bool CollisionInTile(const Vector2 v1, const Vector2 v2);
    bool CollisionRecTile(const Rectangle rec, const Vector2 tile);
    bool CollisionCircleRec(const Vector2 circleCenter, float circleRadius, const Rectangle rectangle);
    bool CollisionLineRec(Vector2 p1, Vector2 p2, Rectangle rec);

    Rectangle SizeRec(Rectangle rec); // Get the rectangle size (width, height) divided by TILE_SIZE
}

#endif