#include "spawner.hpp"

#include "grid_helper.hpp"

void Spawner::UpdateWave()
{
    if (wave.start && !wave.inProgress)
    {
        wave.inProgress = true;
        wave.start = false;
    }

    // Condition to start wave
    if (IsKeyPressed(KEY_S))
        wave.start = true;
};

void Spawner::Draw()
{
    GH::DrawRec(GetRec(), color);
};

void Spawner::GenerateEnemy(std::vector<std::shared_ptr<Enemy>> &enemies, AStar::CoordinateList &path)
{
    timer += GetFrameTime();
    if (timer >= wave.interval)
    {
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