#include "level_manager.hpp"

void LevelManager::GenerateLevel(GridMap& levelMap)
{
    level.map = levelMap;
    for (int x = 0; x < levelMap.size(); ++x)
    {
        for (int y = 0; y < levelMap[x].size(); ++y)
        {
            Vector2 currentPosition {static_cast<float>(x), static_cast<float>(y)};
            if (levelMap[x][y].item == Item::BASE)
            {
                std::shared_ptr<Base> base = std::make_shared<Base>(currentPosition);
                level.map[x][y].obj = base;
                level.base = base;
            }
            else if (levelMap[x][y].item == Item::ROAD)
            {
                std::shared_ptr<Road> road = std::make_shared<Road>(currentPosition);
                level.map[x][y].obj = road;
                level.roads.push_back(road);
            }
            else if (levelMap[x][y].item == Item::PLATFORM)
            {
                level.map[x][y].obj = std::make_shared<Platform>(currentPosition);
                level.GetObj<Platform>(x, y)->signal.Connect(listener);
            }
            else if (levelMap[x][y].item == Item::SPAWN_POINT)
            {
                std::shared_ptr<SpawnPoint> spawn = std::make_shared<SpawnPoint>(currentPosition, enemies);
                level.map[x][y].obj = spawn;
                level.spawnPoint = spawn;
            }
        }
    }

    pathFinding.SetGenerator(level.map);
};

void LevelManager::HandleSignalEvent(Signal::EventData eventData)
{
    switch (eventData.event)
    {
    case Signal::Event::ACTIVATING_PLATFORM:
        // Get platform in eventData.position and set it isSelectingTower to true
        // Then set previous platform to false
        static Vector2 prevPlatformPos {eventData.position};

        level.GetObj<Platform>(eventData.position)->Activate();
        if (!Tile::CollisionInTile(prevPlatformPos, eventData.position))
        {
            level.GetObj<Platform>(prevPlatformPos)->Deactivate();
        }

        prevPlatformPos = eventData.position;
        break;
    default:
        break;
    }
};

void LevelManager::Update()
{
    for (int x = 0; x < level.map.size(); ++x)
    {
        for (int y = 0; y < level.map[x].size(); ++y)
        {
            if (level.map[x][y].item == Item::PLATFORM) level.GetObj<Platform>(x, y)->Update();
        }
    }

    LevelManager::HandleWave();

    for (auto& enemy : enemies)
    {
        enemy->Update();
    }
};

void LevelManager::Draw()
{
    for (int x = 0; x < level.map.size(); ++x)
    {
        for (int y = 0; y < level.map[x].size(); ++y)
        {
            if (level.map[x][y].item == Item::PLATFORM) level.GetObj<Platform>(x, y)->Draw();
            if (level.map[x][y].item == Item::ROAD) level.GetObj<Road>(x, y)->Draw();
            if (level.map[x][y].item == Item::BASE) level.GetObj<Base>(x, y)->Draw();
            if (level.map[x][y].item == Item::SPAWN_POINT) level.GetObj<SpawnPoint>(x, y)->Draw();
        }
    }

    for (auto& enemy : enemies)
    {
        enemy->Draw();
    }
};