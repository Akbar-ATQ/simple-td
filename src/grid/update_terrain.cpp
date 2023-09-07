#include "update_terrain.hpp"

void UpdateSpawner(const std::shared_ptr<Spawner> spawner, Level &level, Grid &grid)
{
    spawner->UpdateWave();
    if (spawner->wave.inProgress)
    {
        auto path = level.pathFinding.GetPath(
            {spawner->position.grid.x, spawner->position.grid.y},
            {level.basePosition.x, level.basePosition.y}
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
    platform->ActivateOnClick();
};

void UpdateTerrain(Grid &grid, Level &level)
{
    if (grid.terrainId == TerrainID::SPAWNER)
        UpdateSpawner(grid.GetTerrain<Spawner>(), level, grid);
    else if (grid.terrainId == TerrainID::PLATFORM)
        UpdatePlatform(grid.GetTerrain<Platform>());
};