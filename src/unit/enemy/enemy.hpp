#ifndef UNIT_ENEMY_HPP
#define UNIT_ENEMY_HPP

#include "global_data.hpp"
#include "entity.hpp"

#include "raylib.h"
#include "AStar.hpp"

class Enemy : public Entity
{
public:
    Enemy(Vec2i gridPos, Vec2f localPos, AStar::CoordinateList &path)
    : Entity{Position{gridPos, localPos}, 20, RED}, path{path}
    {};
    ~Enemy() = default;

    Vec2f GetCenter();

    // Return exp.
    int TakeDamage(int damage);
    int GetHP();

    void Move();

    void Draw();

private:
    struct Path
    {
        AStar::CoordinateList coordinate;
        int index {0};
    };
    Path path;

    int hp {5};
    const int defaultHP {hp};
    float speed {55};
    int expPool {10};
};

#endif