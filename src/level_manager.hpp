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

    void GenerateEnemy(int waveEnemies, float interval, bool& waveInProgress, AStar::CoordinateList& path)
    {
        timer += GetFrameTime();
        if (timer >= interval)
        {
            Vector2 enemyStartingPosition {data.position.x + (GetRandomValue(0, 15) / 100.0f), data.position.y + (GetRandomValue(5, 40) / 100.0f)};

            enemies.push_back(std::make_shared<Enemy>(enemyStartingPosition, path));
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
    void HandleWave();

    // task: handle battle

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

    struct Wave
    {
        bool start {false};
        bool inProgress {false};
        int enemies {15};
        float interval {0.3};
    };
    Wave wave;

    std::vector<std::shared_ptr<Enemy>> enemies;

    PathFinding pathFinding;

    std::shared_ptr<Signal::Listener> listener = std::make_shared<Signal::Listener>();
};

#endif