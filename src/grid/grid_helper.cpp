#include "grid_helper.hpp"

void GH::DrawGrid()
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
    DrawCircle(v.x * GRID_SIZE, v.y * GRID_SIZE, radius, color);
};

void GH::DrawCircLines(const Vec2f v, const float radius, const Color color)
{
    DrawCircleLines(v.x * GRID_SIZE, v.y * GRID_SIZE, radius * GRID_SIZE, color);
};

void GH::DrawTriangleInCenter(const Vec2i gridPos, float size, const Color color)
{
    Vector2 realPos = RealPosition(gridPos).CastVec2Ray();
    float halfGrid = GRID_SIZE / 2;
    Vector2 v1 {(realPos.x + halfGrid), (realPos.y + halfGrid) - size};
    Vector2 v2 {(realPos.x + halfGrid) - size, (realPos.y + halfGrid) + size};
    Vector2 v3 {(realPos.x + halfGrid) + size, (realPos.y + halfGrid) + size};
    DrawTriangle(v1, v2, v3, color);
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

Vec2f GH::RealPosition(const Vec2i v)
{
    return {static_cast<float>(v.x * GRID_SIZE), static_cast<float>(v.y * GRID_SIZE)};
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

Vec2f GH::MergeReal(const Position &position)
{
    return RealPosition(MergeGridLocalPosition(position.grid, position.local));
};

bool GH::OutsideMap(const int x, const int y)
{
    if (x < 0 || y < 0 || x >= MAP_SIZE.x || y >= MAP_SIZE.y)
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
