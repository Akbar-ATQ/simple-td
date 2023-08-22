#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "global_data.hpp"
#include "tile.hpp"
#include "enemy.hpp"
#include "tower.hpp"
#include "platform.hpp"
#include "base.hpp"
#include "road.hpp"
#include "spawner.hpp"
#include "path_finding.hpp"

#include "event_signal.hpp"
#include "event_list.hpp"

#include "AStar.hpp"

#include <vector>
#include <memory>
#include <variant>

// Make it work for battle first then clean up and commit
// and finally use this code to handle other stuff too (primaryEntity update/draw)

#include <iostream>

struct Vec2i
{
    int x;
    int y;

    Vec2i() : x(0), y(0) {};
    Vec2i(int x, int y) : x(x), y(y) {};

    Vec2i operator+(const Vec2i& other) const { return Vec2i(x + other.x, y + other.y); };
    Vec2i operator-(const Vec2i& other) const { return Vec2i(x - other.x, y - other.y); };
    Vec2i operator*(const int scalar) const { return Vec2i(x * scalar, y * scalar); };
    Vec2i operator/(const int divisor) const { return Vec2i(x / divisor, y / divisor); };

    bool operator==(const Vec2i& other) const { return x == other.x && y == other.y; };
    bool operator!=(const Vec2i& other) const { return !(*this == other); };
    bool operator<(const Vec2i& other) const { return x < other.x && y < other.y; };
    bool operator<=(const Vec2i& other) const { return x <= other.x && y <= other.y; };
    bool operator>(const Vec2i& other) const { return x > other.x && y > other.y; };
    bool operator>=(const Vec2i& other) const { return x >= other.x && y >= other.y; };
};

class Level
{
public:
    Level() = default;
    ~Level() = default;

    PathFinding pathFinding;
    Vec2i basePos;

    class Grid
    {
    public:
        Grid(Level &level, int x, int y) : level{level}, position{x, y} {};
        ~Grid() = default;

        // what is in [x][y], map handle that
        const Vec2i position;

        std::shared_ptr<Event::Signal> listener;

        // ---------- Entity ---------- //

        TerrainID terrainId {TerrainID::EMPTY};

        std::variant<
            std::shared_ptr<Base>,
            std::shared_ptr<Road>,
            std::shared_ptr<Spawner>,
            std::shared_ptr<Platform>
        > terrain;

        std::vector<std::shared_ptr<Enemy>> enemies;
        std::vector<std::shared_ptr<Tower>> towers;
        std::vector<std::shared_ptr<Tower::Bullet>> bullets;

        // ------------------------------ //

        template<typename T>
        std::shared_ptr<T> GetTerrain() { return std::get<std::shared_ptr<T>>(terrain); }; // Any static items (road/platform/base/spawn)
        // GetContents(); // enemies & bullets
        bool IsEmpty()
        {
            if (terrainId == TerrainID::EMPTY && (enemies.empty() && towers.empty() && bullets.empty())) return true;
            return false;
        };

        void UpdateEnemies();

        void TowerDetection();

        void UpdateBullets();

        void HandleCollision();

        void UpdateSpawner();

        void UpdatePlatform()
        {
            std::shared_ptr<Platform> platform = GetTerrain<Platform>();
            platform->Update();
        };

        void OnPlatformActivated(const Event::PlatformActivated platform)
        {
            static Vector2 prevPlatformPos {platform.position};

            if (!Tile::CollisionInTile(prevPlatformPos, platform.position))
            {
                level.map[prevPlatformPos.x][prevPlatformPos.y]->GetTerrain<Platform>()->Deactivate();
            }

            prevPlatformPos = platform.position;
        };

        void UpdateTerrain()
        {
            if (terrainId == TerrainID::SPAWNER) UpdateSpawner();
            else if (terrainId == TerrainID::PLATFORM) UpdatePlatform();
        };

        void Update()
        {
            // [todo]
            // Make the call here like this:
            // UpdateTerrain();
            // UpdateUnit();
            // HandleCollision();

            UpdateTerrain();

            UpdateEnemies();
            UpdateBullets();
            HandleCollision();
            TowerDetection(); // Rename it to UpdateTower()
        };

        void Draw();

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

    private:
        Level& level;
    };

    // This enemies is to handle enemy->Draw()
    // Drawing enemy in cell will result in enemy partially gone when it
    // overlap between two cells, because the other cell don't have the enemy to draw,
    // or if the enemy ownership are shared between cells, the enemy->Draw() will be called twice.
    std::vector<std::weak_ptr<Enemy>> enemies;

    std::vector<std::vector<std::unique_ptr<Grid>>> map;

    void InitializeLevel(LevelData &levelData);

    void Update()
    {
        for (int x = 0; x < map.size(); ++x)
        {
            for (int y = 0; y < map[x].size(); ++y)
            {
                if (!map[x][y]->IsEmpty())
                {
                    map[x][y]->Update();
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
                if (!map[x][y]->IsEmpty())
                {
                    map[x][y]->Draw();
                    // for (auto& enemy : map[x][y].enemies)
                    // {
                    //     enemy->Draw();
                    // }
                    // [TODO] need to create virtual calss Entity that have method Update() and Draw()
                    // so we can Draw() entity here
                    // std::visit([](auto& entity)
                    // {
                    //     entity->Draw();
                    // }, map[x][y].primaryEntity);
                    // or just let cell draw itself
                }
            }
        }

        // ---------- Handle drawing enemy and removing it if expire ---------- //
        bool enemyExpire {false};
        for (const auto &enemy : enemies)
        {
            const auto lockedEnemy = enemy.lock();
            if (lockedEnemy) lockedEnemy->Draw();
            else enemyExpire = true;
        }

        if (enemyExpire)
        {
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                [](const auto &enemy)
                {
                    return enemy.expired();
                }), enemies.end());
        }
        // ------------------------------ //
    };
};

#endif