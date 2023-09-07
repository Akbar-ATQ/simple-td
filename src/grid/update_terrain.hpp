#ifndef GRID_UPDATE_TERRAIN_HPP
#define GRID_UPDATE_TERRAIN_HPP

#include "level.hpp"

void UpdateSpawner(const std::shared_ptr<Spawner> spawner, Level &level, Grid &grid);

void UpdatePlatform(std::shared_ptr<Platform> platform);

void UpdateTerrain(Grid &grid, Level &level);

#endif