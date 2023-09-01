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
    : Entity{gridPos, localPos, 20, RED},
    m_path{path}
    {
        // [BUG]
        // when the path direction is changing, it don't perserve the original offset
        // the result is the enemies look like lining and don't look very good
        // for (auto& coordinate : path)
        // {
        //     m_path.coordinate.push_back({
        //         coordinate.x + (m_rec.x - std::floor(m_rec.x)),
        //         coordinate.y + (m_rec.y - std::floor(m_rec.y))
        //     });
        // }
    };
    ~Enemy() = default;

    // struct Data : public Entity
    // {
    //     Data(Vec2i gridPos, Vec2f localPos, float size, Color color, int hp, float speed)
    //     : Entity{gridPos, localPos, size, color}, hp{hp}, speed{speed} {};

    //     int hp;
    //     float speed;
    // };
    int hp {5};
    float speed {55};

    // Data data;

    void TakeDamage(int damage) { hp -= damage; };

    void Move()
    {
        if (m_path.index < m_path.coordinate.size())
        {
            int targetX = m_path.coordinate[m_path.index].x;
            int targetY = m_path.coordinate[m_path.index].y;

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
                position.local.x < 0 || position.local.y < 0
                )
            {
                MoveGrid(this, {targetX, targetY});
            }

            if (position.grid.x == targetX && position.grid.y == targetY)
            {
                m_path.index++;
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
    Path m_path;
};

#endif