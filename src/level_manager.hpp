#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include "platform.hpp"
#include "enemy.hpp"
#include "path_finding.hpp"
#include "entity_data.hpp"
#include "global_data.hpp"
#include "tile.hpp"
#include "grid_map.hpp"

#include "signal.hpp"

#include "raylib.h"
#include "AStar.hpp"
#include "QuadTree.hpp"

#include <vector>
#include <functional>
#include <memory>

class Road
{
public:
    Road(Vector2 pos) : data{pos, TILE_SIZE, BLUE} {};

    EntityData data;

    void Draw() { Tile::DrawRec(data.GetRec(), data.color); };
};

class Base
{
public:
    Base(Vector2 pos) : data{pos, TILE_SIZE, RED} {};

    EntityData data;

    void Draw() { Tile::DrawRec(data.GetRec(), data.color); };
};

#include <iostream>

class SpawnPoint
{
public:
    SpawnPoint(Vector2 pos, std::vector<std::shared_ptr<Enemy>>& enemies, Grid& grid) : data{pos, TILE_SIZE, PURPLE}, enemies{enemies}, grid{grid} {};

    EntityData data;

    void Draw() { Tile::DrawRec(data.GetRec(), data.color); };

    void GenerateEnemy(int waveEnemies, float interval, bool& waveInProgress, AStar::CoordinateList& path)
    {
        timer += GetFrameTime();
        if (timer >= interval)
        {
            Vector2 enemyStartingPosition {data.position.x + (GetRandomValue(0, 15) / 100.0f), data.position.y + (GetRandomValue(5, 40) / 100.0f)};

            std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(enemyStartingPosition, path);
            enemies.push_back(enemy);
            grid.map[static_cast<int>(enemy->data.position.x)][static_cast<int>(enemy->data.position.y)].enemies.push_back(enemy);
            // std::cout << " enemies " << grid.map[static_cast<int>(enemy->data.position.x)][static_cast<int>(enemy->data.position.y)].IsEmpty();

            generatedEnemy += 1;
            timer = 0.0f;
            if (generatedEnemy >= waveEnemies)
            {
                waveInProgress = false;
            }
        }
    };

private:
    int generatedEnemy {0};
    float timer {0};

    std::vector<std::shared_ptr<Enemy>>& enemies;
    Grid& grid;
};

// class CombatManager
// {
// public:
//     CombatManager();
//     ~CombatManager();

//     // LevelManager own it
//     // enemy and tower have reference to it

//     std::vector<std::weak_ptr<Enemy>> GetEnemyInArea(radius) // used by tower to get enemies within its radius
//     {
//         q_enemies.GetInArea(radius);
//     };

//     void AddBullet(bullet) // call by tower
//     {
//         q_bullets.insert(bullet)
//     };

//     void UpdateBullet(); // call bullet.Update()

//     void Update() // call by LevelManager
//     {
//         // get tower range
//         // check enemies against tower range
//         // if collided ask tower to shoot to enemy direction
//         // get bullets from tower

//         // enemies in quadtree
//         // tower search enemy within radius squared
//         // the return is a vector of enemies
//         // returned enemies then check collision against bullets
//         // bullet update pos -> enemy check collision with bullet line from prevPos + currentPos -> if not collided update position in quadtree

//         for (auto& tower : towers)
//         {
//             q_enemies.GetInArea(tower.radius);
//             tower->shoot(enemiesInRange) // tower then pick target from enemies and shoot bullet toward taget
//             q_bullets = tower.GetBullet()
//         }

//         // update bullet pos here //

//         for (enemies)
//         {
//             bullets = q_bullets.GetInArea(enemy.detect)
//             for (bullet : bullets)
//             {
//                 if (collision(enemy, bulletLine))
//                 {
//                     enemy.TakeDamage(bullet.damage);
//                     // erase bullet
//                 }
//             }
//         }
//         // update q_bullets
//     };

//     void HandleSignalEvent(Signal::EventData eventData)
//     {
//         // handle remove/update enemy/bullet
//         // endX = rec.x + static_cast<int>(rec.width / TILE_SIZE)
//         // endY = rec.y + static_cast<int>(rec.height / TILE_SIZE);
//         // for (int x = rec.x; x <= endX; ++x)
//         // {
//         //     for (int y = rec.y; y <= endY; ++y)
//         //     {
//         //         items.push_back(map[x][y].GetItems);
//         //     }
//         // }
//     };

// private:
//     // hold 
//     QuadTree& q_enemies; // reference to LevelManager enemies
//     QuadTree q_bullets {{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, 8, 4};
// };

// ------------------------------ //

class LevelManager
{
public:
    LevelManager()
    {
        listener->SetCallback(std::bind(&LevelManager::HandleSignalEvent, this, std::placeholders::_1));
    };
    ~LevelManager() = default;

    void GenerateLevel(GridMap& levelMap);
    void HandleWave();

    // void Combat()
    // {
    //     // get tower range
    //     // check enemies against tower range
    //     // if collided ask tower to shoot to enemy direction
    //     // get bullets from tower

    //     // enemies in quadtree
    //     // tower search enemy within radius squared
    //     for (auto& tower : towers)
    //     {
    //         q_enemies.GetInArea(tower.radius);
    //         tower->shoot(enemiesInRange)
    //         q_bullets = tower.GetBullet()
    //     }
    //     // the return is a vector of enemies
    //     // returned enemies then check collision against bullets
    //     for (enemies)
    //     {
    //         bullets = q_bullets.GetInArea(enemy.detect)
    //         // update bullet pos
    //         for (bullet : bullets)
    //         {
    //             if (collision(enemy, bulletLine))
    //             {
    //                 enemy.TakeDamage(bullet.damage);
    //                 // erase bullet
    //             }
    //             else // update q_bullet
    //         }
    //     }
    //     // bullet update pos -> enemy check collision with bullet line from prevPos + currentPos -> if not collided update position in quadtree
    // };

    void HandleSignalEvent(Signal::EventData eventData);

    void Update();
    void Draw();

private:
    class Level
    {
        GridMap map;
        // SpawnPoint, Base, Road and Platform are already exist in GridMap
        // But sometimes there a need to get obj based on its position
        // And sometimes there a need to get obj while not knowing its position
        std::shared_ptr<SpawnPoint> spawnPoint;
        std::shared_ptr<Base> base;
        std::vector<std::shared_ptr<Road>> roads;
        std::vector<std::shared_ptr<Platform>> platforms;
        LevelManager* levelManager;

        void InitializeMap();
    public:
        Level(LevelManager* levelManager) : levelManager{levelManager} {};

        void SetMap(GridMap& newMap);
        GridMap& GetMap();

        template<typename T>
        std::shared_ptr<T> GetObj(int x, int y);
        template<typename T>
        std::shared_ptr<T> GetObj(Vector2 v);
        template<typename T>
        std::shared_ptr<T> GetObj(); // Get obj by type, but it expensive

        std::shared_ptr<SpawnPoint> GetSpawnPoint();
        std::shared_ptr<Base> GetBase();
        std::vector<std::shared_ptr<Road>> GetRoads();
        std::vector<std::shared_ptr<Platform>> GetPlatforms();
    };
    Level level {this};

    Grid grid;

    struct Wave
    {
        bool start {false};
        bool inProgress {false};
        int enemies {15};
        float interval {0.3};
    };
    Wave wave;

    QuadTree q_enemies {{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, 8, 4};
    std::vector<std::shared_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Tower>> towers;

    PathFinding pathFinding;

    std::shared_ptr<Signal::Listener> listener = std::make_shared<Signal::Listener>();
};

#endif