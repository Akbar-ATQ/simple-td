#ifndef GLOBAL_DATA_HPP
#define GLOBAL_DATA_HPP

#include "raylib.h"

#include <vector>
#include <memory>

enum class TerrainID
{
    EMPTY = 0,
    ROAD = 1,
    BASE = 2,
    SPAWNER = 3,
    PLATFORM = 4,
};

using LevelData = std::vector<std::vector<TerrainID>>;

enum Mode
{
    MAIN_MENU,
    PLAY,
    LEVEL_EDITOR,
};

extern Mode mode;

template<typename T>
struct Vec2
{
    T x;
    T y;

    Vec2() : x(0), y(0) {};
    Vec2(T x, T y) : x(x), y(y) {};

    // Operator with other Vec2.
    Vec2 operator+(const Vec2 &v) const { return Vec2(x + v.x, y + v.y); };
    Vec2 operator-(const Vec2 &v) const { return Vec2(x - v.x, y - v.y); };
    Vec2 operator*(const Vec2 &v) const { return Vec2(x * v.x, y * v.y); };
    Vec2 operator/(const Vec2 &v) const
    {
        Vec2 result;

        if (v.x != 0)
            result.x = x / v.x;
        else
            result.x = 0;

        if (v.y != 0)
           result.y = y / v.y;
        else
            y = 0;

        return result;
    };

    Vec2 &operator+=(const Vec2 &v) { x += v.x; y += v.y; return *this; };
    Vec2 &operator-=(const Vec2 &v) { x -= v.x; y -= v.y; return *this; };
    Vec2 &operator*=(const Vec2 &v) { x *= v.x; y *= v.y; return *this; };
    Vec2 &operator/=(const Vec2 &v)
    {
        if (v.x != 0)
            x /= v.x;
        else
            x = 0;

        if (v.y != 0)
           y /= v.y;
        else
            y = 0;

        return *this;
    };

    // Operator with signle scalar <T>.
    Vec2 operator+(const T n) const { return Vec2(x + n, y + n); };
    Vec2 operator-(const T n) const { return Vec2(x - n, y - n); };
    Vec2 operator*(const T n) const { return Vec2(x * n, y * n); };
    Vec2 operator/(const T n) const
    {
        if (n != 0)
            return Vec2(x / n, y / n);
        return Vec2(0, 0);
    };

    Vec2 &operator+=(const T n) { x += n; y += n; return *this; };
    Vec2 &operator-=(const T n) { x -= n; y -= n; return *this; };
    Vec2 &operator*=(const T n) { x *= n; y *= n; return *this; };
    Vec2 &operator/=(const T n)
    {
        if (n != 0)
        {
            x /= n;
            y /= n;
        }
        else
        {
            x = 0;
            y = 0;
        }

        return *this;
    };

    // Comparison operator.
    bool operator==(const Vec2 &v) const { return x == v.x && y == v.y; };
    bool operator!=(const Vec2 &v) const { return !(*this == v); };
    bool operator<(const Vec2 &v) const { return x < v.x && y < v.y; };
    bool operator<=(const Vec2 &v) const { return x <= v.x && y <= v.y; };
    bool operator>(const Vec2 &v) const { return x > v.x && y > v.y; };
    bool operator>=(const Vec2 &v) const { return x >= v.x && y >= v.y; };

    template <typename N>
    Vec2<N> Cast() const { return { static_cast<N>(x), static_cast<N>(y) }; };

    // Cast to Vector2 raylib
    Vector2 CastVec2Ray() const { return {static_cast<float>(x), static_cast<float>(y)}; };
};

using Vec2i = Vec2<int>;
using Vec2f = Vec2<float>;

struct Position
{
    Vec2i grid;
    Vec2f local; // Position inside grid.
};

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

const int GRID_SIZE = 40;
const Vec2i MAP_SIZE = {SCREEN_WIDTH / GRID_SIZE, SCREEN_HEIGHT / GRID_SIZE};

#endif