#include "grid_helper.hpp"

void GH::Draw()
{
    for (int x = 0; x < MAP_SIZE.x; ++x)
    {
        for (int y = 0; y < MAP_SIZE.y; ++y)
        {
            DrawRectangleLinesEx({
                    GH::RealPosition(x),
                    GH::RealPosition(y),
                    static_cast<float>(GRID_SIZE),
                    static_cast<float>(GRID_SIZE)
                },
                0.5,
                BLACK
            );
        }
    }
};

void GH::DrawRec(const Rectangle rec, const Color color)
{
    DrawRectangleRec({rec.x, rec.y, rec.width, rec.height}, color);
};

void GH::DrawRecInCenter(const Rectangle rec, const Color color)
{
    DrawRectangleRec({
            GH::GridPosition(rec.x) + ((GRID_SIZE / 2) - (rec.width / 2)),
            GH::GridPosition(rec.y) + ((GRID_SIZE / 2) - (rec.height / 2)),
            rec.width,
            rec.height
        },
        color
    );
};

void GH::DrawCirc(const Vec2f v, const float radius, const Color color)
{
    DrawCircle(v.x * GRID_SIZE, v.y * GRID_SIZE, radius * GRID_SIZE, color);
};

void GH::DrawCircLines(const Vec2f v, const float radius, const Color color)
{
    DrawCircleLines(v.x * GRID_SIZE, v.y * GRID_SIZE, radius * GRID_SIZE, color);
};

// Turn real position into grid position.
Vec2i GH::GridPosition(const Vec2f v)
{
    return {(static_cast<int>(v.x) / GRID_SIZE), (static_cast<int>(v.y) / GRID_SIZE)};
};

Vec2i GH::GridPosition(const Vector2 v)
{
    return {(static_cast<int>(v.x) / GRID_SIZE), (static_cast<int>(v.y) / GRID_SIZE)};
};

int GH::GridPosition(const float pos)
{
    return static_cast<int>(pos) / GRID_SIZE;
};

Vec2f GH::RealPosition(const Vec2f v)
{
    return {(v.x * GRID_SIZE), (v.y * GRID_SIZE)};
};

float GH::RealPosition(const float pos)
{
    return pos * GRID_SIZE;
};

// The result is the digit represent grid position and fraction represent local position in percentage,
// for example: x = 1.3 mean grid[1] 30% from left side.
Vec2f GH::MergeGridLocalPosition(const Vec2i gridPos, const Vec2f localPos)
{
    Vec2f position = gridPos.Cast<float>() + (localPos / GRID_SIZE);
    return position;
};

// Merge grid and local position then get the real position is quite common.
Vec2f GH::MergeReal(const Vec2i gridPos, const Vec2f localPos)
{
    return RealPosition(MergeGridLocalPosition(gridPos, localPos));
};

bool GH::OutsideMap(const int x, const int y)
{
    if (x < 0 || y < 0 || x > MAP_SIZE.x || y > MAP_SIZE.y)
        return true;
    return false;
};

bool GH::OutsideMap(const Vec2i gridPos)
{
    if (gridPos < Vec2i{0, 0} || gridPos > MAP_SIZE)
        return true;
    return false;
};

bool GH::ClickGrid(const Vec2i gridPos)
{
    return (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && GH::GridPosition(GetMousePosition()) == gridPos);
};

bool GH::CollisionRecGrid(const Rectangle rec, const Vec2i gridPos)
{
    Rectangle gridCell {static_cast<float>(gridPos.x * GRID_SIZE), static_cast<float>(gridPos.y * GRID_SIZE), GRID_SIZE, GRID_SIZE};
    if (CheckCollisionRecs(rec, gridCell))
        return true;
    return false;
};

bool GH::CollisionCircleRec(const Vec2f circleCenter, const float circleRadius, const Rectangle rectangle)
{
    // Raylib CircleRecCollision are not accurate for this case.
    // This code is provided by ChatGPT

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

bool GH::CollisionLineRec(Vec2f p1, Vec2f p2, Rectangle rec)
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
    if (CheckCollisionLines(p1.CastVec2Ray(), p2.CastVec2Ray(), rectTopLeft, rectTopRight, &intersectPoint) ||
        CheckCollisionLines(p1.CastVec2Ray(), p2.CastVec2Ray(), rectTopRight, rectBottomRight, &intersectPoint) ||
        CheckCollisionLines(p1.CastVec2Ray(), p2.CastVec2Ray(), rectBottomRight, rectBottomLeft, &intersectPoint) ||
        CheckCollisionLines(p1.CastVec2Ray(), p2.CastVec2Ray(), rectBottomLeft, rectTopLeft, &intersectPoint))
    {
        return true;
    }

    return false;
};
