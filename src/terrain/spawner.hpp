

// class Spawner
// {
// public:
//     Spawner(Vector2 pos, std::vector<std::shared_ptr<Enemy>>& enemies, Grid& grid) : data{pos, TILE_SIZE, PURPLE}, enemies{enemies}, grid{grid} {};

//     EntityData data;

//     void Draw() { Tile::DrawRec(data.GetRec(), data.color); };

//     void GenerateEnemy(int waveEnemies, float interval, bool& waveInProgress, AStar::CoordinateList& path)
//     {
//         timer += GetFrameTime();
//         if (timer >= interval)
//         {
//             Vector2 enemyStartingPosition {data.position.x + (GetRandomValue(0, 15) / 100.0f), data.position.y + (GetRandomValue(5, 40) / 100.0f)};

//             std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(enemyStartingPosition, path);
//             enemies.push_back(enemy);
//             grid.map[static_cast<int>(enemy->data.position.x)][static_cast<int>(enemy->data.position.y)].enemies.push_back(enemy);
//             // std::cout << " enemies " << grid.map[static_cast<int>(enemy->data.position.x)][static_cast<int>(enemy->data.position.y)].IsEmpty();

//             generatedEnemy += 1;
//             timer = 0.0f;
//             if (generatedEnemy >= waveEnemies)
//             {
//                 waveInProgress = false;
//             }
//         }
//     };

// private:
//     int generatedEnemy {0};
//     float timer {0};

//     std::vector<std::shared_ptr<Enemy>>& enemies;
//     Grid& grid;
// };
#include "enemy.hpp"
#include "entity_data.hpp"
#include "global_data.hpp"

#include "level.hpp"

#include "raylib.h"
#include "AStar.hpp"

#include <vector>

#include <iostream>

class Spawner
{
public:
    Spawner(Vector2 pos) : data{pos, TILE_SIZE, PURPLE} {};

    EntityData data;

    struct Wave
    {
        bool start {false};
        bool inProgress {false};
        int enemies {15};
        float interval {0.3};
    };
    Wave wave;

    void Update()
    {
        // grid
        // spawner.Update()
        // if (wave,inProgress)
        //      spawner.GenerateEnemy()
        if (wave.start && !wave.inProgress)
        {
            wave.inProgress = true;
            wave.start = false;
        }

        // if (wave.inProgress)
        // {
        //     // auto path = pathFinding.GetPath(
        //     //     {static_cast<int>(level.GetSpawnPoint()->data.position.x), static_cast<int>(level.GetSpawnPoint()->data.position.y)},
        //     //     {static_cast<int>(level.GetBase()->data.position.x), static_cast<int>(level.GetBase()->data.position.y)}
        //     // );
        //     // level.GetSpawnPoint()->GenerateEnemy(wave.enemies, wave.interval, wave.inProgress, path);
        // }

        // Condition to start wave
        if (IsKeyPressed(KEY_S)) wave.start = true;
    };

    void Draw() { Tile::DrawRec(data.GetRec(), data.color); };

    void GenerateEnemy(std::vector<std::weak_ptr<Enemy>> &levelEnemies, std::vector<std::shared_ptr<Enemy>> &enemies, AStar::CoordinateList &path)
    {
        timer += GetFrameTime();
        if (timer >= wave.interval)
        {
            Vector2 enemyStartingPosition {data.position.x + (GetRandomValue(0, 15) / 100.0f), data.position.y + (GetRandomValue(5, 40) / 100.0f)};

            std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(enemyStartingPosition, path);
            enemies.push_back(enemy);
            levelEnemies.push_back(enemy);

            generatedEnemy += 1;
            timer = 0.0f;
            if (generatedEnemy >= wave.enemies)
            {
                generatedEnemy = 0;
                wave.inProgress = false;
            }
        }
    };

private:
    int generatedEnemy {0};
    float timer {0};
};