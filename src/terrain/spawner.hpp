#ifndef TERRAIN_SPAWNER_HPP
#define TERRAIN_SPAWNER_HPP

#include "enemy.hpp"
#include "entity.hpp"
#include "global_data.hpp"
#include "grid_helper.hpp"

#include "level.hpp"

#include "raylib.h"
#include "AStar.hpp"

#include <vector>

class Spawner : public Entity
{
public:
    Spawner(Vec2i gridPos) : Entity{Position{gridPos, {0, 0}}, GRID_SIZE, PURPLE} {};

    // EntityData data;

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

    void Draw() { GH::DrawRec(GetRec(), color); };

    void GenerateEnemy(std::vector<std::shared_ptr<Enemy>> &enemies, AStar::CoordinateList &path)
    {
        timer += GetFrameTime();
        if (timer >= wave.interval)
        {
            // Vector2 enemyStartingPosition {data.position.grid.x + (GetRandomValue(0, 15) / 100.0f), data.position.grid.y + (GetRandomValue(5, 40) / 100.0f)};
            Vec2i enemyGridPos = position.grid;
            Vec2f enemyLocalPos {static_cast<float>(GetRandomValue(5, 15)), static_cast<float>(GetRandomValue(5, 15))};

            std::shared_ptr<Enemy> enemy = std::make_shared<Enemy>(enemyGridPos, enemyLocalPos, path);
            enemies.push_back(enemy);

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

#endif