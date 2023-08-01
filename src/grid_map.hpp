#ifndef GRIDMAP_HPP
#define GRIDMAP_HPP

#include "global_data.hpp"
#include "tile.hpp"
#include "enemy.hpp"
#include "tower.hpp"
#include "platform.hpp"

#include <vector>
#include <memory>
#include <variant>

// Make it work for battle first then clean up and commit
// and finally use this code to handle other stuff too (primaryEntity update/draw)

#include <iostream>

class Grid
{
public:
    Grid()
    {
        SetMap();
    };
    ~Grid() = default;

    struct Vec2i
    {
        int x;
        int y;

        Vec2i() : x(0), y(0) {};
        Vec2i(int x, int y) : x(x), y(y) {};

        Vec2i operator+(const Vec2i& other) const { return Vec2i(x + other.x, y + other.y); };
        Vec2i operator-(const Vec2i& other) const { return Vec2i(x - other.x, y - other.y); };
        Vec2i operator*(int scalar) const { return Vec2i(x * scalar, y * scalar); };
        Vec2i operator/(int divisor) const { return Vec2i(x / divisor, y / divisor); };

        bool operator==(const Vec2i& other) const { return x == other.x && y == other.y; };
        bool operator!=(const Vec2i& other) const { return !(*this == other); };
        bool operator<(const Vec2i& other) const { return x < other.x && y < other.y; };
        bool operator<=(const Vec2i& other) const { return x <= other.x && y <= other.y; };
        bool operator>(const Vec2i& other) const { return x > other.x && y > other.y; };
        bool operator>=(const Vec2i& other) const { return x >= other.x && y >= other.y; };
    };

    void SetMap()
    {
        for (int x = 0; x < MAP_SIZE.x; ++x)
        {
            std::vector<Map> mapY;

            for (int y = 0; y < MAP_SIZE.y; ++y)
            {
                Map map = {*this, x, y};
                mapY.push_back(map);
            }

            map.push_back(mapY);
        }
    };

    class Map
    {
    public:
        Map(Grid& grid, int x, int y) : grid{grid}, position{x, y} {};
        ~Map() = default;

        // what is in [x][y], map handle that

        Grid& grid;
        const Vec2i position;

        // ---------- Entity ---------- //

        enum PrimaryEntityID
        {
            EMPTY,
            ROAD,
            PLATFORM,
            BASE,
            SPAWN_POINT,
        };

        std::variant<
            std::shared_ptr<Base>,
            std::shared_ptr<Road>,
            std::shared_ptr<SpawnPoint>,
            std::shared_ptr<Platform>
        > primaryEntity;

        PrimaryEntityID primaryEntityId {EMPTY};

        std::vector<std::shared_ptr<Enemy>> enemies;
        std::vector<std::shared_ptr<Tower>> towers;
        std::vector<std::shared_ptr<Tower::Bullet>> bullets;

        // ------------------------------ //

        template<typename T>
        std::shared_ptr<T> GetPrimaryEntity() { return std::get<std::shared_ptr<T>>(primaryEntity); }; // Any static items (road/platform/base/spawn)
        // GetContents(); // enemies & bullets
        bool IsEmpty()
        {
            if (primaryEntityId == EMPTY || (enemies.empty() && towers.empty() && bullets.empty())) return true;
            return false;
        };

        void UpdateEnemies()
        {
            std::vector<std::shared_ptr<Enemy>> enemiesToMove;
            for (auto& enemy : enemies)
            {
                enemy->Update();
                if (!Tile::CollisionInTile(enemy->data.position, {position.x, position.y})) enemiesToMove.push_back(enemy);
            }

            if (!enemiesToMove.empty())
            {
                for (const auto& enemyToMove : enemiesToMove)
                {
                    int newX = static_cast<int>(enemyToMove->data.position.x);
                    int newY = static_cast<int>(enemyToMove->data.position.y);
                    grid.map[newX][newY].enemies.push_back(enemyToMove);
                }

                grid.map[position.x][position.y].enemies.erase(std::remove_if(grid.map[position.x][position.y].enemies.begin(), grid.map[position.x][position.y].enemies.end(),
                    [this](const std::shared_ptr<Enemy> enemy)
                    {
                        if (enemy->data.hp <= 0) return true;
                        int newX = static_cast<int>(enemy->data.position.x);
                        int newY = static_cast<int>(enemy->data.position.y);
                        return newX != position.x || newY != position.y;
                    }), grid.map[position.x][position.y].enemies.end());
            }
        };

        void TowerDetection()
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
                        if (grid.map[x][y].primaryEntityId == ROAD && !grid.map[x][y].enemies.empty())
                        {
                            enemiesInRange.insert(
                                enemiesInRange.end(),
                                grid.map[x][y].enemies.begin(),
                                grid.map[x][y].enemies.end()
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

        void UpdateBullets()
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
                        grid.map[newX][newY].bullets.push_back(bulletToMove);
                    else bulletToMove->RemoveFromTower(bulletToMove);
                }

                grid.map[position.x][position.y].bullets.erase(
                    std::remove_if(grid.map[position.x][position.y].bullets.begin(), grid.map[position.x][position.y].bullets.end(),
                    [this](const std::shared_ptr<Tower::Bullet> bullet)
                    {
                        int newX = static_cast<int>(bullet->pos.x);
                        int newY = static_cast<int>(bullet->pos.y);
                        return newX != position.x || newY != position.y;
                    }), grid.map[position.x][position.y].bullets.end());
            }
        };

        void HandleCollision()
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

        void Update()
        {
            UpdateEnemies();
            UpdateBullets();
            HandleCollision();
            TowerDetection();
            // HandleCollision();
            // if (primaryEntityId == PLATFORM) GetPrimaryEntity<Platform>()->Update();
        };

        // void HandleSignalEvent(Signal::EventData eventData)
        // {
        //     // handle Signal::Event::TOWER_ADDED;
        //     // handle remove/update enemy/bullet
        //     // Get item in area rec
        //     // endX = rec.x + static_cast<int>(rec.width / TILE_SIZE)
        //     // endY = rec.y + static_cast<int>(rec.height / TILE_SIZE);
        //     // for (int x = rec.x; x <= endX; ++x)
        //     // {
        //     //     for (int y = rec.y; y <= endY; ++y)
        //     //     {
        //     //         items.push_back(map[x][y].GetItems);
        //     //     }
        //     // }
        // };
    };

    std::vector<std::vector<Map>> map;

    void UpdateGridCell(int x, int y)
    {
        // if (map[x][y].enemies.size() != 0) UpdateEnemies(x, y);
        // if (/* check if tower exist */) UpdateTower(x, y);
        // UpdateBullets(x, y);
        // HanldeCollision??
    };

    void Update()
    {
        for (int x = 0; x < map.size(); ++x)
        {
            for (int y = 0; y < map[x].size(); ++y)
            {
                if (!map[x][y].IsEmpty())
                {
                    map[x][y].Update();
                    // UpdateGridCell(x, y);
                    // how about just call map[x][y].Update() then let cell handle updating stuff inside it,
                    // including moving entity to other cell
                }
            }
        }
    };

    void Draw()
    {
        for (int x = 0; x < map.size(); ++x)
        {
            for (int y = 0; y < map[x].size(); ++y)
            {
                if (!map[x][y].IsEmpty())
                {
                    for (auto& enemy : map[x][y].enemies)
                    {
                        enemy->Draw();
                    }
                    // [TODO] need to create virtual calss Entity that have method Update() and Draw()
                    // so we can Draw() entity here
                    // std::visit([](auto& entity)
                    // {
                    //     entity->Draw();
                    // }, map[x][y].primaryEntity);
                }
            }
        }
    };
};

#endif