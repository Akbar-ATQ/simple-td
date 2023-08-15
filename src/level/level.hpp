#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "global_data.hpp"
#include "tile.hpp"
#include "enemy.hpp"
#include "tower.hpp"
#include "platform.hpp"
#include "base.hpp"
#include "road.hpp"

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

    class Grid
    {
    public:
        Grid(Level &level, int x, int y) : level{level}, position{x, y} {};
        ~Grid() = default;

        // what is in [x][y], map handle that
        const Vec2i position;

        // ---------- Entity ---------- //

        TerrainID terrainId {TerrainID::EMPTY};

        std::variant<
            std::shared_ptr<Base>,
            std::shared_ptr<Road>,
            std::shared_ptr<SpawnPoint>,
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

        void Update()
        {
            UpdateEnemies();
            UpdateBullets();
            HandleCollision();
            TowerDetection();
            // HandleCollision();
            // if (terrainId == PLATFORM) GetPrimaryEntity<Platform>()->Update();
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

    std::vector<std::vector<Grid>> map;

    void InitializeLevel(LevelData &levelData);

    void Update()
    {
        for (int x = 0; x < map.size(); ++x)
        {
            for (int y = 0; y < map[x].size(); ++y)
            {
                if (!map[x][y].IsEmpty())
                {
                    map[x][y].Update();
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
                    map[x][y].Draw();
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
    };
};

#endif