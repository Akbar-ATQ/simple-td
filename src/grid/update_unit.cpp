#include "update_unit.hpp"

#include "grid_helper.hpp"

void UpdateEnemies(Grid &grid, Level::Manager &level)
{
    std::vector<std::weak_ptr<Enemy>> enemiesToMove;

    bool expired {false};

    for (auto &enemy : grid.enemies)
    {
        auto lockedEnemy = enemy.lock();
        if (lockedEnemy)
        {
            lockedEnemy->Move();

            if (lockedEnemy->position.grid.x != grid.position.x || lockedEnemy->position.grid.y != grid.position.y)
                enemiesToMove.push_back(lockedEnemy);
        }
        else
            expired = true;
    }

    if (!enemiesToMove.empty() || expired)
    {
        for (auto &enemyToMove : enemiesToMove)
        {
            auto lockedEnemy = enemyToMove.lock();
            if (lockedEnemy)
            {
                int newX = lockedEnemy->position.grid.x;
                int newY = lockedEnemy->position.grid.y;
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
                            int newX = lockedEnemy->position.grid.x;
                            int newY = lockedEnemy->position.grid.y;
                            return newX != grid.position.x || newY != grid.position.y;
                        }
                        else // enemy expired
                            return true;
                    }),
            level.map[grid.position.x][grid.position.y]->enemies.end());
    }
};

void UpdateTower(Grid &grid, Level::Manager &level)
{
    // [Idea] make enemy have int id to make it easy targeting first/last

    std::shared_ptr<Tower> tower = grid.GetTerrain<Platform>()->GetTower();

    std::vector<std::shared_ptr<Enemy>> enemiesInRange;

    auto addEnemyToRange = [&enemiesInRange, &level](const int x, const int y)
    {
        if (!GH::OutsideMap(x, y) && level.map[x][y]->terrainId == TerrainID::ROAD && !level.map[x][y]->enemies.empty())
        {
            for (auto &enemy : level.map[x][y]->enemies)
            {
                auto lockedEnemy = enemy.lock();
                if (lockedEnemy)
                    enemiesInRange.push_back(lockedEnemy);
            }
        }
    };

    int towerDetectionRange = static_cast<int>(tower->detectionRange);

    Vec2i startRange {
        tower->position.grid.x - towerDetectionRange,
        tower->position.grid.y - towerDetectionRange
    };
    Vec2i endRange {
        tower->position.grid.x + towerDetectionRange,
        tower->position.grid.y + towerDetectionRange
    };

    for (int x = startRange.x; x < endRange.x; ++x)
    {
        for (int y = startRange.y; y < endRange.y; ++y)
        {
            addEnemyToRange(x, y);
        }
    }

    tower->Shoot(enemiesInRange);
    if (!tower->GetBullets().empty())
        tower->MoveBullets(grid.bullets);
};

void UpdateBullets(Grid &grid, Level::Manager &level)
{
    std::vector<std::shared_ptr<Bullet>> bulletsToMove;
    for (auto &bullet : grid.bullets)
    {
        bullet->Move();

        if (bullet->position.grid != grid.position)
            bulletsToMove.push_back(bullet);
    }

    if (!bulletsToMove.empty())
    {
        for (auto &bulletToMove : bulletsToMove)
        {
            int newX = bulletToMove->position.grid.x;
            int newY = bulletToMove->position.grid.y;

            if (!GH::OutsideMap(newX, newY))
                level.map[newX][newY]->bullets.push_back(bulletToMove);
        }

        level.map[grid.position.x][grid.position.y]->bullets.erase(
            std::remove_if(
                level.map[grid.position.x][grid.position.y]->bullets.begin(),
                level.map[grid.position.x][grid.position.y]->bullets.end(),
                [grid](const std::shared_ptr<Bullet> bullet)
                    {
                        return bullet->position.grid.x != grid.position.x || bullet->position.grid.y != grid.position.y;
                    }),
            level.map[grid.position.x][grid.position.y]->bullets.end());
    }
};

void HandleCollision(Grid &grid, Level::Manager &level)
{
    // Bullet take enemies in grid cells around it and check collision against it.
    // the reason is because enemy can be between two grid cells but only one cell own it,
    // so there a possibility bullet will collided with enemy in neighboor cell
    // but it don't participate in collision.
    std::vector<std::shared_ptr<Bullet>> removedBullets;

    for (auto &bullet : grid.bullets)
    {
        std::vector<Vec2i> neighboorCells = {
            {bullet->position.grid.x - 1, bullet->position.grid.y},
            {bullet->position.grid.x + 1, bullet->position.grid.y},
            {bullet->position.grid.x, bullet->position.grid.y - 1},
            {bullet->position.grid.x, bullet->position.grid.y + 1}
        };

        std::vector<std::weak_ptr<Enemy>> enemies;

        if (!grid.enemies.empty())
        {
            for (auto &enemy : grid.enemies)
            {
                enemies.push_back(enemy);
            }
        }

        for (const auto &neighboor : neighboorCells)
        {
            if (!GH::OutsideMap(neighboor.x, neighboor.y) && !level.map[neighboor.x][neighboor.y]->enemies.empty())
            {
                for (auto &enemy : level.map[neighboor.x][neighboor.y]->enemies)
                {
                    enemies.push_back(enemy);
                }
            }
        }

        for (auto &enemy : enemies)
        {
            auto lockedEnemy = enemy.lock();
            if (lockedEnemy)
            {
                if (CheckCollisionPointRec(GH::MergeReal(bullet->position).CastVec2Ray(), lockedEnemy->GetRec()))
                {
                    lockedEnemy->TakeDamage(bullet->damage);
                    removedBullets.push_back(bullet);
                }
            }
        }
    }

    if (!removedBullets.empty())
    {
        auto isRemoved = [&removedBullets](const std::shared_ptr<Bullet> &bullet)
        {
            return std::find(removedBullets.begin(), removedBullets.end(), bullet) != removedBullets.end();
        };

        grid.bullets.erase(std::remove_if(grid.bullets.begin(), grid.bullets.end(), isRemoved), grid.bullets.end());
    }
};

void UpdateUnit(Grid &grid, Level::Manager &level)
{
    if (!grid.enemies.empty())
        UpdateEnemies(grid, level);

    if (grid.terrainId == TerrainID::PLATFORM && grid.GetTerrain<Platform>()->HasTower())
        UpdateTower(grid, level);

    if (!grid.bullets.empty())
        UpdateBullets(grid, level);

    if (!grid.enemies.empty() && !grid.bullets.empty())
        HandleCollision(grid, level);
};
