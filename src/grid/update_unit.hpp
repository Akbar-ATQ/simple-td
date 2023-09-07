#ifndef GRID_UPDATE_UNIT_HPP
#define GRID_UPDATE_UNIT_HPP

#include "level.hpp"
#include "grid.hpp"
#include "grid_helper.hpp"

void UpdateEnemies(Grid &grid, Level &level);

void UpdateTower(Grid &grid, Level &level);

void UpdateBullets(Grid &grid, Level &level);

void HandleCollision(Grid &grid, Level &level);

void UpdateUnit(Grid &grid, Level &level);

#endif