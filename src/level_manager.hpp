#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include "platform.hpp"
#include "enemy.hpp"
#include "path_finding.hpp"
#include "entity_data.hpp"
#include "global_data.hpp"
#include "tile.hpp"
#include "level.hpp"

#include "signal.hpp"

#include "raylib.h"
#include "AStar.hpp"

#include <vector>
#include <functional>
#include <memory>

class LevelManager
{
public:
    LevelManager()
    {
        // listener->SetCallback(std::bind(&LevelManager::HandleSignalEvent, this, std::placeholders::_1));
    };
    ~LevelManager() = default;

    void GenerateLevel(LevelData &levelData);
    void HandleWave();

    void HandleSignalEvent(Signal::EventData eventData);

    void Update();
    void Draw();

private:
    // class Level
    // {
    //     GridMap map;
    //     // SpawnPoint, Base, Road and Platform are already exist in GridMap
    //     // But sometimes there a need to get obj based on its position
    //     // And sometimes there a need to get obj while not knowing its position
    //     std::shared_ptr<SpawnPoint> spawnPoint;
    //     std::shared_ptr<Base> base;
    //     std::vector<std::shared_ptr<Road>> roads;
    //     std::vector<std::shared_ptr<Platform>> platforms;
    //     LevelManager *levelManager;

    //     void InitializeMap();
    // public:
    //     Level(LevelManager* levelManager) : levelManager{levelManager} {};

    //     void SetMap(GridMap& newMap);
    //     GridMap& GetMap();

    //     template<typename T>
    //     std::shared_ptr<T> GetObj(int x, int y);
    //     template<typename T>
    //     std::shared_ptr<T> GetObj(Vector2 v);
    //     template<typename T>
    //     std::shared_ptr<T> GetObj(); // Get obj by type, but it expensive

    //     std::shared_ptr<SpawnPoint> GetSpawnPoint();
    //     std::shared_ptr<Base> GetBase();
    //     std::vector<std::shared_ptr<Road>> GetRoads();
    //     std::vector<std::shared_ptr<Platform>> GetPlatforms();
    // };
    Level level;

    // Grid grid;

    struct Wave
    {
        bool start {false};
        bool inProgress {false};
        int enemies {15};
        float interval {0.3};
    };
    Wave wave;

    // QuadTree q_enemies {{ 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT }, 8, 4};
    // std::vector<std::shared_ptr<Enemy>> enemies;
    // std::vector<std::shared_ptr<Tower>> towers;

    // PathFinding pathFinding;

    // std::shared_ptr<Signal::Listener> listener = std::make_shared<Signal::Listener>();
};

#endif
