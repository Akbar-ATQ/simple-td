#ifndef TERRAIN_SPAWNER_HPP
#define TERRAIN_SPAWNER_HPP

#include "enemy.hpp"
#include "entity.hpp"
#include "global_data.hpp"

#include "raylib.h"
#include "AStar.hpp"

#include <vector>
#include <memory>

class Spawner : public Entity
{
public:
    Spawner(Vec2i gridPos) : Entity{Position{gridPos, {0, 0}}, GRID_SIZE, PURPLE} {};

    struct Wave
    {
        bool start {false};
        bool inProgress {false};
        int enemies {15};
        float interval {0.3};
    };
    Wave wave;

    void UpdateWave();

    void Draw();

    void GenerateEnemy(std::vector<std::shared_ptr<Enemy>> &enemies, AStar::CoordinateList &path);

private:
    int generatedEnemy {0};
    float timer {0};
};

#endif