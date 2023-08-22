#include "level.hpp"

// #include "base.hpp"
// #include "platform.hpp"
// #include "road.hpp"
// #include "spawner.hpp"
#include <iostream>
#include <utility>

void Level::InitializeLevel(LevelData &levelData)
{
    pathFinding.SetGenerator(levelData);
    for (int x = 0; x < MAP_SIZE.x; ++x)
    {
        std::vector<std::unique_ptr<Grid>> gridY;

        for (int y = 0; y < MAP_SIZE.y; ++y)
        {
            Vector2 currentPosition{static_cast<float>(x), static_cast<float>(y)};

            std::unique_ptr<Grid> grid = std::make_unique<Grid>(*this, x, y);

            switch (levelData[x][y])
            {
                case TerrainID::ROAD:
                {
                    grid->terrainId = TerrainID::ROAD;
                    grid->terrain = std::make_shared<Road>(currentPosition);
                    break;
                }
                case TerrainID::PLATFORM:
                {
                    grid->terrainId = TerrainID::PLATFORM;

                    std::shared_ptr<Platform> platform = std::make_shared<Platform>(currentPosition);

                    grid->listener = Event::Signal::Create(grid.get());
                    grid->listener->Connect<Event::PlatformActivated, Level::Grid>(Event::PlatformActivated(), &Level::Grid::OnPlatformActivated);
                    grid->listener->Connect(platform->eventEmitter);

                    grid->terrain = platform;
                    break;
                }
                case TerrainID::SPAWNER:
                {
                    grid->terrainId = TerrainID::SPAWNER;
                    grid->terrain = std::make_shared<Spawner>(currentPosition);
                    break;
                }
                case TerrainID::BASE:
                {
                    grid->terrainId = TerrainID::BASE;
                    grid->terrain = std::make_shared<Base>(currentPosition);
                    basePos = {x, y};
                    break;
                }
            }

            gridY.push_back(std::move(grid));
        }

        map.push_back(std::move(gridY));
    }
};