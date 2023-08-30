#include "tile.hpp"

#include "global_data.hpp"

#include "raylib.h"

#include <algorithm>

// ---------- Handle tile related task/calculation ---------- //
void Tile::Draw()
{
    for (int x = 0; x < MAP_SIZE.x; ++x)
    {
        for (int y = 0; y < MAP_SIZE.y; ++y)
        {
            DrawRectangleLinesEx({
                    static_cast<float>(x * TILE_SIZE),
                    static_cast<float>(y * TILE_SIZE),
                    static_cast<float>(TILE_SIZE),
                    static_cast<float>(TILE_SIZE)
                },
                0.5,
                BLACK
            );
        }
    }
};

void Tile::DrawRec(const Rectangle rec, const Color color)
{
    DrawRectangleRec({(rec.x * TILE_SIZE), (rec.y * TILE_SIZE), rec.width, rec.height}, color);
};

void Tile::DrawRecOnCenter(const Rectangle rec, const Color color)
{
    DrawRectangleRec({
            (rec.x * TILE_SIZE) + ((TILE_SIZE / 2) - (rec.width / 2)),
            (rec.y * TILE_SIZE) + ((TILE_SIZE / 2) - (rec.height / 2)),
            rec.width,
            rec.height
        },
        color
    );
};

void Tile::DrawCirc(const Vector2 v, const float radius, const Color color)
{
    DrawCircle(v.x * TILE_SIZE, v.y * TILE_SIZE, radius * TILE_SIZE, color);
};

void Tile::DrawCircLines(const Vector2 v, const float radius, const Color color)
{
    DrawCircleLines(v.x * TILE_SIZE, v.y * TILE_SIZE, radius * TILE_SIZE, color);
};

Vector2 Tile::Position(const Vector2 v)
{
    return {(v.x / TILE_SIZE), (v.y / TILE_SIZE)};
};

Vector2 Tile::RealPosition(const Vector2 v)
{
    return {(v.x * TILE_SIZE), (v.y * TILE_SIZE)};
};

float Tile::RealPosition(const float pos)
{
    return pos * TILE_SIZE;
};

bool Tile::OutsideMap(const int x, const int y)
{
    if (x < 0 || y < 0 || x > MAP_SIZE.x || y > MAP_SIZE.y) return true;
    return false;
};

bool Tile::ClickTile(const Vector2 tilePos)
{
    return (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CollisionInTile(Position(GetMousePosition()), tilePos));
};

bool Tile::CollisionInTile(const Vector2 v1, const Vector2 v2)
{
    return (static_cast<int>(v1.x) == static_cast<int>(v2.x) && static_cast<int>(v1.y) == static_cast<int>(v2.y));
};

bool Tile::CollisionRecTile(const Rectangle rec, const Vector2 tile)
{
    Rectangle grid {(tile.x * TILE_SIZE), (tile.y * TILE_SIZE), TILE_SIZE, TILE_SIZE};
    if (CheckCollisionRecs(rec, grid))
        return true;
    return false;
};

bool Tile::CollisionCircleRec(const Vector2 circleCenter, float circleRadius, const Rectangle rectangle)
{
    // Raylib CircleRecCollision are not accurate for this case.
    // This code is provided by CharGPT

    // Find the closest point on the rectangle to the circle
    float closestX = std::max(rectangle.x, std::min(circleCenter.x, rectangle.x + rectangle.width));
    float closestY = std::max(rectangle.y, std::min(circleCenter.y, rectangle.y + rectangle.height));

    // Calculate the distance between the closest point and the circle center
    float distanceX = closestX - circleCenter.x;
    float distanceY = closestY - circleCenter.y;
    float distanceSquared = (distanceX * distanceX) + (distanceY * distanceY);

    // Check if the distance is less than or equal to the radius squared
    float radiusSquared = circleRadius * circleRadius;
    bool isColliding = distanceSquared <= radiusSquared;

    return isColliding;
};

bool Tile::CollisionLineRec(Vector2 p1, Vector2 p2, Rectangle rec)
{
    // This code provided by ChatGPT with some minor edit & correction

    // Check if either endpoint is inside the rectangle
    if ((p1.x >= rec.x && p1.x <= rec.x + rec.width && p1.y >= rec.y && p1.y <= rec.y + rec.height) ||
        (p2.x >= rec.x && p2.x <= rec.x + rec.width && p2.y >= rec.y && p2.y <= rec.y + rec.height))
    {
        return true;
    }

    // Check intersection with each side of the rectangle
    Vector2 rectTopLeft = { rec.x, rec.y };
    Vector2 rectTopRight = { rec.x + rec.width, rec.y };
    Vector2 rectBottomLeft = { rec.x, rec.y + rec.height };
    Vector2 rectBottomRight = { rec.x + rec.width, rec.y + rec.height };

    Vector2 intersectPoint {0, 0};
    if (CheckCollisionLines(p1, p2, rectTopLeft, rectTopRight, &intersectPoint) ||
        CheckCollisionLines(p1, p2, rectTopRight, rectBottomRight, &intersectPoint) ||
        CheckCollisionLines(p1, p2, rectBottomRight, rectBottomLeft, &intersectPoint) ||
        CheckCollisionLines(p1, p2, rectBottomLeft, rectTopLeft, &intersectPoint))
    {
        return true;
    }

    return false;
};


Rectangle Tile::SizeRec(Rectangle rec)
{
    return {rec.x, rec.y, (rec.width / TILE_SIZE), (rec.height / TILE_SIZE)};
};