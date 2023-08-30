#ifndef UPDATE_UNIT_HPP
#define UPDATE_UNIT_HPP

#include "level.hpp"
#include "grid.hpp"

void UpdateEnemies(Grid &grid, Level &level)
{
    std::vector<std::weak_ptr<Enemy>> enemiesToMove;

    bool expired {false};

    for (auto& enemy : grid.enemies)
    {
        auto lockedEnemy = enemy.lock();
        if (lockedEnemy)
        {
            lockedEnemy->Update();

            if (lockedEnemy->data.gridPosition.x != grid.position.x || lockedEnemy->data.gridPosition.y != grid.position.y)
            {
                enemiesToMove.push_back(lockedEnemy);
            }
        }
        else
            expired = true;
    }

    if (!enemiesToMove.empty() || expired)
    {
        for (auto& enemyToMove : enemiesToMove)
        {
            auto lockedEnemy = enemyToMove.lock();
            if (lockedEnemy)
            {
                int newX = lockedEnemy->data.gridPosition.x;
                int newY = lockedEnemy->data.gridPosition.y;
                level.map[newX][newY]->enemies.push_back(lockedEnemy);
            }
        }

        level.map[grid.position.x][grid.position.y]->enemies.erase(
            std::remove_if(
                level.map[grid.position.x][grid.position.y]->enemies.begin(),
                level.map[grid.position.x][grid.position.y]->enemies.end(),
                [grid](const std::weak_ptr<Enemy> enemy)
                    {
                        auto lockedEnemy = enemy.lock();
                        if (lockedEnemy)
                        {
                            int newX = lockedEnemy->data.gridPosition.x;
                            int newY = lockedEnemy->data.gridPosition.y;
                            return newX != grid.position.x || newY != grid.position.y;
                        }
                        else // enemy expired
                            return true;
                    }),
            level.map[grid.position.x][grid.position.y]->enemies.end());
    }
};

// move it to some other place later
bool OutsideMap(const int x, const int y)
{
    if (x < 0 || y < 0 || x > MAP_SIZE.x || y > MAP_SIZE.y)
        return true;
    return false;
};

void UpdateTower(Grid &grid, Level &level)
{
    // [Idea] make enemy have int id to make it easy targeting first/last

    std::shared_ptr<Tower> tower = grid.GetTerrain<Platform>()->GetTower();
    std::vector<std::shared_ptr<Enemy>> enemiesInRange;

    Vec2i startRange {
        tower->data.position.x - tower->data.detectionRange,
        tower->data.position.y - tower->data.detectionRange
    };
    Vec2i endRange {
        tower->data.position.x + tower->data.detectionRange,
        tower->data.position.y + tower->data.detectionRange
    };

    for (int x = startRange.x; x < endRange.x; ++x)
    {
        for (int y = startRange.y; y < endRange.y; ++y)
        {
            if (!OutsideMap(x, y) && level.map[x][y]->terrainId == TerrainID::ROAD && !level.map[x][y]->enemies.empty())
            {
                for (auto& enemy : level.map[x][y]->enemies)
                {
                    auto lockedEnemy = enemy.lock();
                    if (lockedEnemy)
                        enemiesInRange.push_back(lockedEnemy);
                }
            }

        }
    }

    tower->Shoot(enemiesInRange);
    if (!tower->GetBullets().empty())
    {
        std::vector<std::shared_ptr<Tower::Bullet>> towerBullets = tower->GetBullets();
        grid.bullets.insert(grid.bullets.end(), towerBullets.begin(), towerBullets.end());
    }
};

void UpdateBullets(Grid &grid, Level &level)
{
    std::vector<std::shared_ptr<Tower::Bullet>> bulletsToMove;
    for (auto& bullet : grid.bullets)
    {
        bullet->Update();
        if (!Tile::CollisionInTile(bullet->pos, {grid.position.x, grid.position.y}))
            bulletsToMove.push_back(bullet);
    }

    if (!bulletsToMove.empty())
    {
        for (const auto& bulletToMove : bulletsToMove)
        {
            int newX = static_cast<int>(bulletToMove->pos.x);
            int newY = static_cast<int>(bulletToMove->pos.y);

            if ((newX >= 0 && newY >= 0) && (newX < MAP_SIZE.x && newY < MAP_SIZE.y))
                level.map[newX][newY]->bullets.push_back(bulletToMove);
            else bulletToMove->RemoveFromTower(bulletToMove);
        }

        level.map[grid.position.x][grid.position.y]->bullets.erase(
            std::remove_if(
                level.map[grid.position.x][grid.position.y]->bullets.begin(),
                level.map[grid.position.x][grid.position.y]->bullets.end(),
                [grid](const std::shared_ptr<Tower::Bullet> bullet)
                    {
                        int newX = static_cast<int>(bullet->pos.x);
                        int newY = static_cast<int>(bullet->pos.y);
                        return newX != grid.position.x || newY != grid.position.y;
                    }),
            level.map[grid.position.x][grid.position.y]->bullets.end());
    }
};

void HandleCollision(Grid &grid)
{
    for (auto &enemy : grid.enemies)
    {
        for (auto &bullet : grid.bullets)
        {
            auto lockedEnemy = enemy.lock();
            if (lockedEnemy)
            {
                // [todo] [improvement]
                // bullet take enemies in grid cells around it and check collision against it
                // the reason is because enemy can be between two grid cells but only one cell own it
                // so there a possibility bullet will collided with enemy in neighbor cell
                // but it don't participate in collision
                if (CheckCollisionPointRec(bullet->pos, lockedEnemy->data.GetRec()))
                {
                    lockedEnemy->TakeDamage(bullet->damage);
                    bullet->RemoveFromTower(bullet);
                }
            }
        }
    }
};

void UpdateUnit(Grid &grid, Level &level)
{
    if (!grid.enemies.empty())
        UpdateEnemies(grid, level);

    if (grid.terrainId == TerrainID::PLATFORM && grid.GetTerrain<Platform>()->HasTower())
        UpdateTower(grid, level);

    if (!grid.bullets.empty())
        UpdateBullets(grid, level);

    if (!grid.enemies.empty() && !grid.bullets.empty())
        HandleCollision(grid);
};

#endif