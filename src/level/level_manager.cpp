#include "level_manager.hpp"

#include "platform.hpp"
#include "base.hpp"
#include "road.hpp"
#include "spawner.hpp"
#include "update_terrain.hpp"
#include "update_unit.hpp"

inline void GenerateTerrain(LevelData &levelData, Grid &grid, Level::Manager &level)
{
    switch (levelData[grid.position.x][grid.position.y])
    {
        case TerrainID::ROAD:
        {
            grid.terrainId = TerrainID::ROAD;
            grid.terrain = std::make_shared<Road>(grid.position);
            break;
        }
        case TerrainID::PLATFORM:
        {
            grid.terrainId = TerrainID::PLATFORM;

            std::shared_ptr<Platform> platform = std::make_shared<Platform>(grid.position);
            level.listener->Connect(platform->event);

            grid.terrain = platform;
            break;
        }
        case TerrainID::SPAWNER:
        {
            grid.terrainId = TerrainID::SPAWNER;
            grid.terrain = std::make_shared<Spawner>(grid.position);
            break;
        }
        case TerrainID::BASE:
        {
            grid.terrainId = TerrainID::BASE;
            grid.terrain = std::make_shared<Base>(grid.position);
            break;
        }
    }
};

void Level::Manager::GenerateLevel(LevelData &levelData)
{
    pathFinding.SetGenerator(levelData);

    // these are to assign basePosition to spawner to be used for path generation.
    std::vector<Vec2i> spawnersPosition;
    Vec2i basePosition;

    for (int x = 0; x < MAP_SIZE.x; ++x)
    {
        std::vector<std::unique_ptr<Grid>> gridY;

        for (int y = 0; y < MAP_SIZE.y; ++y)
        {
            std::unique_ptr<Grid> grid = std::make_unique<Grid>(x, y);

            GenerateTerrain(levelData, *grid, *this);

            if (levelData[x][y] == TerrainID::SPAWNER)
                spawnersPosition.push_back({x, y});
            if (levelData[x][y] == TerrainID::BASE)
                basePosition = {x, y};

            gridY.push_back(std::move(grid));
        }

        map.push_back(std::move(gridY));
    }

    for (const auto &spawnerPosition : spawnersPosition)
    {
        map[spawnerPosition.x][spawnerPosition.y]->GetTerrain<Spawner>()->basePosition = basePosition;
    }
};

void Level::Manager::OnPlatformActivated(const Event::PlatformActivated platform)
{
    static Vec2i prevPlatformGrid {platform.position.grid};

    if (prevPlatformGrid != platform.position.grid)
    {
        map[prevPlatformGrid.x][prevPlatformGrid.y]->GetTerrain<Platform>()->Deactivate();
    }

    prevPlatformGrid = platform.position.grid;
};

void Level::Manager::Clear()
{
    map.clear();
    pathFinding.generator.clearCollisions();
    enemies.clear();
};

void Level::Manager::Update()
{
    for (int x = 0; x < map.size(); ++x)
    {
        for (int y = 0; y < map[x].size(); ++y)
        {
            if (!map[x][y]->IsEmpty())
            {
                UpdateTerrain(*map[x][y], *this);
                UpdateUnit(*map[x][y], *this);
            }
        }
    }

    // temporary code to remove enemy if hp <= 0
    // use signal for proper implementation
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
        [](const auto &enemy)
            {
                if (enemy->GetHP() <= 0)
                    return true;
                return false;
            }),
    enemies.end());
};

void Level::Manager::Draw()
{
    for (int x = 0; x < map.size(); ++x)
    {
        for (int y = 0; y < map[x].size(); ++y)
        {
            if (!map[x][y]->IsEmpty())
            {
                if (map[x][y]->terrainId != TerrainID::EMPTY)
                {
                    std::visit([](const auto &terrain)
                        {
                            terrain->Draw();
                        },
                        map[x][y]->terrain
                    );
                }

                for (const auto &bullet : map[x][y]->bullets)
                {
                    bullet->Draw();
                }
            }
        }
    }

    for (const auto &enemy : enemies)
    {
        enemy->Draw();
    }
};
