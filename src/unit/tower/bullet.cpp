#include "bullet.hpp"

#include "raylib.h"
#include "raymath.h"

void Bullet::Draw()
{
    GH::DrawCirc(GH::MergeGridLocalPosition(position.grid, position.local), size, color);
};

void Bullet::Move()
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

std::shared_ptr<Bullet> Bullet::Create(
    const Vec2i &grid,
    const Vec2f &local,
    const float &speed,
    const float &damage,
    const std::weak_ptr<Enemy> &enemy
)
{
    std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>();
    bullet->position.grid = grid;
    bullet->position.local = local;
    bullet->prevPosition = bullet->position;
    bullet->speed = speed;
    bullet->damage = damage;
    bullet->size = 1.0f;
    bullet->enemy = enemy;
    bullet->color = BLACK;

    return bullet;
};