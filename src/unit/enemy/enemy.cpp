#include "enemy.hpp"

#include "grid_helper.hpp"

Vec2f Enemy::GetCenter()
{
    return {position.local.x + (size / 2), position.local.y + (size / 2)};
};

void Enemy::TakeDamage(int damage)
{
    hp -= damage;
};

int Enemy::GetHP()
{
    return hp;
};

void Enemy::Move()
{
    // When the path direction is changing, it don't perserve the original offset
    // the result is the enemies look like lining and don't look very good.
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

        if (!GH::CollisionRecGrid(GetRec(), position.grid))
        {
            MoveGrid(this, {targetX, targetY});
        }

        if (position.grid.x == targetX && position.grid.y == targetY)
        {
            path.index++;
        }
    }
};

void Enemy::Draw()
{
    GH::DrawRec(GetRec(), color);
};
