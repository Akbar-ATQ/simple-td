#ifndef UNIT_ENEMY_HPP
#define UNIT_ENEMY_HPP

#include "global_data.hpp"
#include "grid_helper.hpp"
#include "entity.hpp"

#include "raylib.h"
#include "AStar.hpp"

class Enemy : public Entity
{
public:
    Enemy(Vec2i gridPos, Vec2f localPos, AStar::CoordinateList& path)
    : Entity{Position{gridPos, localPos}, 20, RED}, path{path}
    {};
    ~Enemy() = default;

    int hp {5};
    float speed {55};

    Vec2f GetCenter()
    {
        return {position.local.x + (size / 2), position.local.y + (size / 2)};
    };

    void TakeDamage(int damage) { hp -= damage; };

    void Move()
    {
        // [BUG]
        // when the path direction is changing, it don't perserve the original offset
        // the result is the enemies look like lining and don't look very good
        if (path.index < path.coordinate.size())
        {
            int targetX = path.coordinate[path.index].x;
            int targetY = path.coordinate[path.index].y;

            if (position.grid.x != targetX)
            {
                if (position.grid.x > targetX)
                    position.local.x -= speed * GetFrameTime();
                else
                    position.local.x += speed * GetFrameTime();
            }
            else if (position.grid.y != targetY)
            {
                if (position.grid.y > targetY)
                    position.local.y -= speed * GetFrameTime();
                else
                    position.local.y += speed * GetFrameTime();
            }

            if (position.local.x > GRID_SIZE || position.local.y > GRID_SIZE ||
                position.local.x < 0 || position.local.y < 0)
            {
                MoveGrid(this, {targetX, targetY});
            }

            if (position.grid.x == targetX && position.grid.y == targetY)
            {
                path.index++;
            }
        }
    };

    void Update()
    {
        Move();
    };
    void Draw()
    {
        GH::DrawRec(GetRec(), color);
    };

private:
    struct Path
    {
        AStar::CoordinateList coordinate;
        int index {0};
    };
    Path path;
};

#endif