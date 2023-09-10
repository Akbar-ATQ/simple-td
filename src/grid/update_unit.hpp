#ifndef GRID_UPDATE_UNIT_HPP
#define GRID_UPDATE_UNIT_HPP

#include "level_manager.hpp"
#include "grid.hpp"
#include "grid_helper.hpp"

void UpdateEnemies(Grid &grid, Level::Manager &level);

void UpdateTower(Grid &grid, Level::Manager &level);

void UpdateBullets(Grid &grid, Level::Manager &level);

void HandleCollision(Grid &grid, Level::Manager &level);

void UpdateUnit(Grid &grid, Level::Manager &level);

#endif