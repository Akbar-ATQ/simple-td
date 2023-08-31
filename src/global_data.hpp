#ifndef GLOBAL_DATA_HPP
#define GLOBAL_DATA_HPP

#include "raylib.h"

#include <vector>
#include <memory>

enum class TerrainID
{
    BASE,
    SPAWNER,
    ROAD,
    PLATFORM,
    EMPTY,
};

using LevelData = std::vector<std::vector<TerrainID>>;

template<typename T>
struct Vec2
{
    T x;
    T y;

    Vec2() : x(0), y(0) {};
    Vec2(T x, T y) : x(x), y(y) {};

    Vec2 operator+(const Vec2 &other) const { return Vec2(x + other.x, y + other.y); };
    Vec2 operator-(const Vec2 &other) const { return Vec2(x - other.x, y - other.y); };
    Vec2 operator*(const T scalar) const { return Vec2(x * scalar, y * scalar); };
    Vec2 operator/(const T divisor) const { return Vec2(x / divisor, y / divisor); };

    Vec2 &operator+=(const Vec2 &other) { x += other.x; y += other.y; return *this; }
    Vec2 &operator-=(const Vec2 &other) { x -= other.x; y -= other.y; return *this; }
    Vec2 &operator*=(const T scalar) { x *= scalar; y *= scalar; return *this; }
    Vec2 &operator/=(const T divisor) { x /= divisor; y /= divisor; return *this; }

    bool operator==(const Vec2 &other) const { return x == other.x && y == other.y; };
    bool operator!=(const Vec2 &other) const { return !(*this == other); };
    bool operator<(const Vec2 &other) const { return x < other.x && y < other.y; };
    bool operator<=(const Vec2 &other) const { return x <= other.x && y <= other.y; };
    bool operator>(const Vec2 &other) const { return x > other.x && y > other.y; };
    bool operator>=(const Vec2 &other) const { return x >= other.x && y >= other.y; };

    template <typename N>
    Vec2<N> Cast() const { return { static_cast<N>(x), static_cast<N>(y) }; }

    // Cast to Vector2 raylib
    Vector2 CastVec2Ray() const { return {static_cast<float>(x), static_cast<float>(y)}; };
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int GRID_SIZE = 40;
const Vec2i MAP_SIZE = {SCREEN_WIDTH / GRID_SIZE, SCREEN_HEIGHT / GRID_SIZE};

#endif