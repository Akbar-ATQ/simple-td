#ifndef GLOBALDATA_HPP
#define GLOBALDATA_HPP

#include "raylib.h"

#include <vector>
#include <memory>

enum class Item
{
    ROAD,
    PLATFORM,
    BASE,
    SPAWN_POINT,
    EMPTY,
};

enum class TerrainID
{
    BASE,
    SPAWNER,
    ROAD,
    PLATFORM,
    EMPTY,
};

using LevelData = std::vector<std::vector<TerrainID>>;

struct Vec2i
{
    int x;
    int y;

    Vec2i() : x(0), y(0) {};
    Vec2i(int x, int y) : x(x), y(y) {};

    Vec2i operator+(const Vec2i& other) const { return Vec2i(x + other.x, y + other.y); };
    Vec2i operator-(const Vec2i& other) const { return Vec2i(x - other.x, y - other.y); };
    Vec2i operator*(const int scalar) const { return Vec2i(x * scalar, y * scalar); };
    Vec2i operator/(const int divisor) const { return Vec2i(x / divisor, y / divisor); };

    bool operator==(const Vec2i& other) const { return x == other.x && y == other.y; };
    bool operator!=(const Vec2i& other) const { return !(*this == other); };
    bool operator<(const Vec2i& other) const { return x < other.x && y < other.y; };
    bool operator<=(const Vec2i& other) const { return x <= other.x && y <= other.y; };
    bool operator>(const Vec2i& other) const { return x > other.x && y > other.y; };
    bool operator>=(const Vec2i& other) const { return x >= other.x && y >= other.y; };
};

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
// [todo] move tile variable to tile.hpp
const int TILE_SIZE = 40;
const Vector2 MAP_SIZE = {SCREEN_WIDTH / TILE_SIZE, SCREEN_HEIGHT / TILE_SIZE};

#endif