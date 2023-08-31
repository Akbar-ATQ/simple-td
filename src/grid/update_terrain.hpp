#ifndef GRID_UPDATE_TERRAIN_HPP
#define GRID_UPDATE_TERRAIN_HPP

#include "level.hpp"

void UpdateSpawner(const std::shared_ptr<Spawner> spawner, Level &level, Grid &grid)
{
    spawner->Update();
    if (spawner->wave.inProgress)
    {
        // [todo] make the spawner hold base position
        auto path = level.pathFinding.GetPath(
            {spawner->gridPosition.x, spawner->gridPosition.y},
            {level.basePos.x, level.basePos.y}
        );
        std::vector<std::shared_ptr<Enemy>> enemies;

        spawner->GenerateEnemy(enemies, path);

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