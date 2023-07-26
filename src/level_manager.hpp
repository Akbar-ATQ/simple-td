#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include "platform.hpp"
#include "enemy.hpp"
#include "path_finding.hpp"
#include "entity_data.hpp"
#include "global_data.hpp"
#include "tile.hpp"

#include "signal.hpp"

#include "raylib.h"
#include "AStar.hpp"

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

class SpawnPoint
{
public:
    SpawnPoint(Vector2 pos, std::vector<std::shared_ptr<Enemy>>& enemies) : data{pos, TILE_SIZE, PURPLE}, enemies{enemies} {};

    EntityData data;

    void Draw() { Tile::DrawRec(data.GetRec(), data.color); };

    void GenerateEnemy(int enemyToGenerate, float interval, bool& waveInProgress, AStar::CoordinateList& path)
    {
        timer += GetFrameTime();
        if (timer >= interval)
        {
            Vector2 enemyStartingPosition {data.position.x + (GetRandomValue(0, 15) / 100.0f), data.position.y + (GetRandomValue(5, 40) / 100.0f)};

            enemies.push_back(std::make_shared<Enemy>(enemyStartingPosition, path));
            generatedEnemy += 1;
            timer = 0.0f;
            if (generatedEnemy >= enemyToGenerate)
            {
                waveInProgress = false;
            }
        }
    };

private:
    int generatedEnemy {0};
    float timer {0};

    std::vector<std::shared_ptr<Enemy>>& enemies;
};

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
    void HandleWave()
    {
        if (wave.start && !wave.inProgress)
        {
            wave.inProgress = true;
            wave.start = false;
        }

        if (wave.inProgress)
        {
            int enemyToGenerate = 15;
            float interval = 0.3;
            auto path = pathFinding.GetPath(
                {static_cast<int>(level.spawnPoint->data.position.x), static_cast<int>(level.spawnPoint->data.position.y)},
                {static_cast<int>(level.base->data.position.x), static_cast<int>(level.base->data.position.y)}
            );
            level.spawnPoint->GenerateEnemy(enemyToGenerate, interval, wave.inProgress, path);
        }

        // Condition to start wave
        if (IsKeyPressed(KEY_S)) wave.start = true;
    };

    void HandleSignalEvent(Signal::EventData eventData);

    void Update();
    void Draw();

private:
    struct Level
    {
        GridMap map;
        std::shared_ptr<SpawnPoint> spawnPoint;
        std::shared_ptr<Base> base;
        std::vector<std::shared_ptr<Road>> roads;

        template<typename T>
        std::shared_ptr<T> GetObj(int x, int y) { return std::get<std::shared_ptr<T>>(map[x][y].obj); };
        template<typename T>
        std::shared_ptr<T> GetObj(Vector2 v ){ return std::get<std::shared_ptr<T>>(map[v.x][v.y].obj); };
    };
    Level level;
    struct Wave
    {
        bool start {false};
        bool inProgress {false};
    };
    Wave wave;

    std::vector<std::shared_ptr<Enemy>> enemies;

    PathFinding pathFinding;

    std::shared_ptr<Signal::Listener> listener = std::make_shared<Signal::Listener>();
};

#endif