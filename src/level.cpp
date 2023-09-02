#include "level.hpp"

#include "platform.hpp"
#include "base.hpp"
#include "road.hpp"
#include "spawner.hpp"
#include "update_terrain.hpp"
#include "update_unit.hpp"

inline void GenerateTerrain(LevelData &levelData, Grid &grid, Level &level)
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

            level.listener->Connect<Event::PlatformActivated, Level>(Event::PlatformActivated(), &Level::OnPlatformActivated);
            level.listener->Connect(platform->eventEmitter);

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

            level.basePos = grid.position;
            break;
        }
    }
};

void Level::GenerateLevel(LevelData &levelData)
{
    pathFinding.SetGenerator(levelData);

    for (int x = 0; x < MAP_SIZE.x; ++x)
    {
        std::vector<std::unique_ptr<Grid>> gridY;

        for (int y = 0; y < MAP_SIZE.y; ++y)
        {
            std::unique_ptr<Grid> grid = std::make_unique<Grid>(x, y);

            GenerateTerrain(levelData, *grid, *this);

            gridY.push_back(std::move(grid));
        }

        map.push_back(std::move(gridY));
    }
};

void Level::OnPlatformActivated(const Event::PlatformActivated platform)
{
    static Vec2i prevPlatformGrid {platform.position.grid};

    if (prevPlatformGrid != platform.position.grid)
    {
        map[prevPlatformGrid.x][prevPlatformGrid.y]->GetTerrain<Platform>()->Deactivate();
    }

    prevPlatformGrid = platform.position.grid;
};

void Level::Update()
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
                if (enemy->hp <= 0)
                    return true;
                return false;
            }),
    enemies.end());
};


// Break it from Level::Draw
// To prevent making Level::Draw too deep
inline void DrawTerrain(Grid &grid)
{
    if (grid.terrainId == TerrainID::BASE)
    {
        grid.GetTerrain<Base>()->Draw();
    }
    else if (grid.terrainId == TerrainID::SPAWNER)
    {
        grid.GetTerrain<Spawner>()->Draw();
    }
    else if (grid.terrainId == TerrainID::ROAD)
    {
        grid.GetTerrain<Road>()->Draw();
    }
    else if (grid.terrainId == TerrainID::PLATFORM)
    {
        grid.GetTerrain<Platform>()->Draw();
    }
};

void Level::Draw()
{
    for (int x = 0; x < map.size(); ++x)
    {
        for (int y = 0; y < map[x].size(); ++y)
        {
            if (!map[x][y]->IsEmpty())
            {
                DrawTerrain(*map[x][y]);
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
