#include "level.hpp"

// #include "base.hpp"
// #include "platform.hpp"
// #include "road.hpp"
// #include "spawner.hpp"
#include <iostream>

void Level::InitializeLevel(LevelData &levelData)
{
    pathFinding.SetGenerator(levelData);
    for (int x = 0; x < MAP_SIZE.x; ++x)
    {
        std::vector<Grid> gridY;

        for (int y = 0; y < MAP_SIZE.y; ++y)
        {
            Vector2 currentPosition{static_cast<float>(x), static_cast<float>(y)};

            Grid grid = {*this, x, y};

            switch (levelData[x][y])
            {
                case TerrainID::ROAD:
                {
                    grid.terrainId = TerrainID::ROAD;
                    grid.terrain = std::make_shared<Road>(currentPosition);
                    break;
                }
                case TerrainID::PLATFORM:
                {
                    grid.terrainId = TerrainID::PLATFORM;
                    grid.terrain = std::make_shared<Platform>(currentPosition);
                    break;
                }
                case TerrainID::SPAWNER:
                {
                    grid.terrainId = TerrainID::SPAWNER;
                    grid.terrain = std::make_shared<Spawner>(currentPosition);
                    break;
                }
                case TerrainID::BASE:
                {
                    grid.terrainId = TerrainID::BASE;
                    grid.terrain = std::make_shared<Base>(currentPosition);
                    basePos = {x, y};
                    break;
                }
            }

            gridY.push_back(grid);
        }

        map.push_back(gridY);
    }
};