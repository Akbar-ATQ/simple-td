#ifndef UNIT_BULLET_HPP
#define UNIT_BULLET_HPP

#include "entity.hpp"
#include "enemy.hpp"
#include "global_data.hpp"

#include "raymath.h"

class Bullet : public Entity
{
public:
    Bullet() = default;
    ~Bullet() = default;

    Position prevPosition;

    std::weak_ptr<Enemy> enemy;

    // ---------- stats ---------- //
    float speed {0};
    Vec2f direction {0, 0};
    float damage {0};
    // ---------------------------- //

    // std::shared_ptr<Event::Signal> hittingEnemy // When hitting enemy, take exp and give it to tower

    void Draw()
    {
        GH::DrawCirc(GH::MergeGridLocalPosition(position.grid, position.local), size, BLACK);
    };

    void Update()
    {
        Move();
    };

    void Move()
    {
        auto lockedEnemy = enemy.lock();
        if (lockedEnemy)
        {
            Vec2f realPos = GH::MergeReal(position);
            Vec2f enemyRealPos = GH::MergeReal(lockedEnemy->position.grid, lockedEnemy->GetCenter());
            Vec2f realDirection = enemyRealPos - realPos;

            Vector2 rayDirection = Vector2Normalize(realDirection.CastVec2Ray());
            direction = {rayDirection.x, rayDirection.y};
        }
        position.local += (direction * speed) * GetFrameTime();

        bool isMovingGrid {false};
        Vec2i newGrid = position.grid;

        if (position.local.x > GRID_SIZE)
        {
            newGrid.x = position.grid.x + 1;
            isMovingGrid = true;
        }
        else if (position.local.y > GRID_SIZE)
        {
            newGrid.y = position.grid.y + 1;
            isMovingGrid = true;
        }
        else if (position.local.x < 0)
        {
            newGrid.x = position.grid.x - 1;
            isMovingGrid = true;
        }
        else if (position.local.y < 0)
        {
            newGrid.y = position.grid.y - 1;
            isMovingGrid = true;
        }

        if (isMovingGrid)
        {
            MoveGrid(this, newGrid);
        }
    };
};

#endif