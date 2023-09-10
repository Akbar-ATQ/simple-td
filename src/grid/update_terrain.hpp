#ifndef GRID_UPDATE_TERRAIN_HPP
#define GRID_UPDATE_TERRAIN_HPP

#include "level_manager.hpp"

void UpdateSpawner(const std::shared_ptr<Spawner> spawner, Level::Manager &level, Grid &grid);

void UpdatePlatform(std::shared_ptr<Platform> platform);

void UpdateTerrain(Grid &grid, Level::Manager &level);

#endif