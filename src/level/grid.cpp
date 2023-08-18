#include "level.hpp"

#include <iostream>

void Level::Grid::UpdateEnemies()
{
    // [todo]
    // Currently enemy will only moved to other grid cell when it position (top left corner) is not inside this grid cell again.
    // it will become problem for bullet collision since bullet check collision against enemy in this grid cell,
    // but there can be an enemy that is between two grid cells but not yet moved.
    // solution: when enemy is between two grid cell, its ownership is shared.
    // how to implement it is another problem.
    // Maye using shared_ptr.use_count() to check the number of owner, and make it a rule that
    // other that is not grid can only have weak_ptr
    // Or I can just use weak_ptr for other cell
    

    // use move semantic
    std::vector<std::shared_ptr<Enemy>> enemiesToMove;
    // std::vector<std::shared_ptr<Enemy>> sharedEnemies;
    for (auto& enemy : enemies)
    {
        enemy->Update();
        if (!Tile::CollisionInTile(enemy->data.position, {position.x, position.y}))
        {
            enemiesToMove.push_back(enemy);
        }

        // check whether enemy is between two grid cells
        // float enemyGridSize = (((enemy->data.size * enemy->data.size) / (TILE_SIZE * TILE_SIZE)) * 100) / 100;

        // if (enemy->data.position.x < position.x ||
        //     enemy->data.position.x + (enemyGridSize * 2) > position.x + 1 ||
        //     enemy->data.position.y < position.y ||
        //     enemy->data.position.y + (enemyGridSize * 2) > position.y + 1)
        // {
        //     share ownership
        // }
    }

    if (!enemiesToMove.empty())
    {
        for (const auto& enemyToMove : enemiesToMove)
        {
            int newX = static_cast<int>(enemyToMove->data.position.x);
            int newY = static_cast<int>(enemyToMove->data.position.y);
            level.map[newX][newY].enemies.push_back(enemyToMove);
        }

        level.map[position.x][position.y].enemies.erase(std::remove_if(level.map[position.x][position.y].enemies.begin(), level.map[position.x][position.y].enemies.end(),
            [this](const std::shared_ptr<Enemy> enemy)
            {
                if (enemy->data.hp <= 0) return true;
                int newX = static_cast<int>(enemy->data.position.x);
                int newY = static_cast<int>(enemy->data.position.y);
                return newX != position.x || newY != position.y;
            }), level.map[position.x][position.y].enemies.end());
    }
};

void Level::Grid::TowerDetection()
{
    for (auto& tower : towers)
    {
        // [Idea] make enemy have int id to make it easy targeting first/last
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
                if (level.map[x][y].terrainId == TerrainID::ROAD && !level.map[x][y].enemies.empty())
                {
                    enemiesInRange.insert(
                        enemiesInRange.end(),
                        level.map[x][y].enemies.begin(),
                        level.map[x][y].enemies.end()
                    );
                }

            }
        }

        tower->Shoot(enemiesInRange);
        if (!tower->GetBullets().empty())
        {
            std::vector<std::shared_ptr<Tower::Bullet>> towerBullets = tower->GetBullets();
            bullets.insert(bullets.end(), towerBullets.begin(), towerBullets.end());
        }
    }
};

void Level::Grid::UpdateBullets()
{
    std::vector<std::shared_ptr<Tower::Bullet>> bulletsToMove;
    for (auto& bullet : bullets)
    {
        bullet->Update();
        if (!Tile::CollisionInTile(bullet->pos, {position.x, position.y})) bulletsToMove.push_back(bullet);
    }

    if (!bulletsToMove.empty())
    {
        for (const auto& bulletToMove : bulletsToMove)
        {
            int newX = static_cast<int>(bulletToMove->pos.x);
            int newY = static_cast<int>(bulletToMove->pos.y);

            if ((newX >= 0 && newY >= 0) && (newX < MAP_SIZE.x && newY < MAP_SIZE.y))
                level.map[newX][newY].bullets.push_back(bulletToMove);
            else bulletToMove->RemoveFromTower(bulletToMove);
        }

        level.map[position.x][position.y].bullets.erase(
            std::remove_if(level.map[position.x][position.y].bullets.begin(), level.map[position.x][position.y].bullets.end(),
            [this](const std::shared_ptr<Tower::Bullet> bullet)
            {
                int newX = static_cast<int>(bullet->pos.x);
                int newY = static_cast<int>(bullet->pos.y);
                return newX != position.x || newY != position.y;
            }), level.map[position.x][position.y].bullets.end());
    }
};

void Level::Grid::HandleCollision()
{
    if (!enemies.empty() && !bullets.empty())
    {
        for (auto& enemy : enemies)
        {
            for (auto& bullet : bullets)
            {
                if (CheckCollisionPointRec(bullet->pos, enemy->data.GetRec()))
                {
                    enemy->TakeDamage(bullet->damage);
                    bullet->RemoveFromTower(bullet);
                    // [Task]
                    // clean up the code
                }
            }
        }
    }
};

#include <iostream>

void Level::Grid::UpdateSpawner()
{
    if (terrainId == TerrainID::SPAWNER)
    {
        std::shared_ptr spawner = GetTerrain<Spawner>();
        spawner->Update();
        if (spawner->wave.inProgress)
        {
            // std::cout << "test";
            auto path = level.pathFinding.GetPath(
                {static_cast<int>(position.x), static_cast<int>(position.y)},
                {static_cast<int>(level.basePos.x), static_cast<int>(level.basePos.y)}
            );
            spawner->GenerateEnemy(level.enemies, enemies, path);
        }
    }
};

void Level::Grid::Draw()
{
    if (terrainId == TerrainID::BASE)
    {
        GetTerrain<Base>()->Draw();
    }
    else if (terrainId == TerrainID::SPAWNER)
    {
        GetTerrain<Spawner>()->Draw();
    }
    else if (terrainId == TerrainID::ROAD)
    {
        GetTerrain<Road>()->Draw();
    }
    else if (terrainId == TerrainID::PLATFORM)
    {
        GetTerrain<Platform>()->Draw();
    }

    // for (const auto &enemy : enemies)
    // {
    //     enemy->Draw();
    // }
};