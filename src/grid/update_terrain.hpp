#ifndef UPDATE_TERRAIN_HPP
#define UPDATE_TERRAIN_HPP

#include "level.hpp"

void UpdateSpawner(const std::shared_ptr<Spawner> spawner, Level &level, Grid &grid)
{
    spawner->Update();
    if (spawner->wave.inProgress)
    {
        // [todo] make the spawner hold base position
        auto path = level.pathFinding.GetPath(
            {static_cast<int>(spawner->data.position.x), static_cast<int>(spawner->data.position.y)},
            {static_cast<int>(level.basePos.x), static_cast<int>(level.basePos.y)}
        );
        std::vector<std::shared_ptr<Enemy>> enemies;

        spawner->NewGenerateEnemy(enemies, path);

        for (auto& enemy : enemies)
        {
            level.enemies.push_back(enemy);
            grid.enemies.push_back(enemy);
        }
    }
};

void UpdatePlatform(std::shared_ptr<Platform> platform)
{
    platform->Update();
};

void UpdateTerrain(Grid &grid, Level &level)
{
    if (grid.terrainId == TerrainID::SPAWNER)
        UpdateSpawner(grid.GetTerrain<Spawner>(), level, grid);
    else if (grid.terrainId == TerrainID::PLATFORM)
        UpdatePlatform(grid.GetTerrain<Platform>());
};

#endif