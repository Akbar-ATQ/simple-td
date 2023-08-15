#include "level_manager.hpp"

void LevelManager::GenerateLevel(LevelData &levelData)
{
    level.InitializeLevel(levelData);
    // pathFinding.SetGenerator(level.GetMap());
};

// void LevelManager::HandleWave()
// {
//     if (wave.start && !wave.inProgress)
//     {
//         wave.inProgress = true;
//         wave.start = false;
//     }

//     if (wave.inProgress)
//     {
//         auto path = pathFinding.GetPath(
//             {static_cast<int>(level.GetSpawnPoint()->data.position.x), static_cast<int>(level.GetSpawnPoint()->data.position.y)},
//             {static_cast<int>(level.GetBase()->data.position.x), static_cast<int>(level.GetBase()->data.position.y)}
//         );
//         level.GetSpawnPoint()->GenerateEnemy(wave.enemies, wave.interval, wave.inProgress, path);
//     }

//     // Condition to start wave
//     if (IsKeyPressed(KEY_S)) wave.start = true;
// };


// #include <iostream>
// void LevelManager::HandleSignalEvent(Signal::EventData eventData)
// {
//     switch (eventData.event)
//     {
//         case Signal::Event::ACTIVATING_PLATFORM:
//         {
//             // Activate platform based on position in eventData
//             // and deactivate previously active platform
//             static Vector2 prevPlatformPos {eventData.position};

//             level.GetObj<Platform>(eventData.position)->Activate();
//             if (!Tile::CollisionInTile(prevPlatformPos, eventData.position))
//             {
//                 level.GetObj<Platform>(prevPlatformPos)->Deactivate();
//             }

//             prevPlatformPos = eventData.position;
//             break;
//         }
//         case Signal::Event::TOWER_ADDED:
//         {
//             towers.push_back(level.GetObj<Platform>(eventData.position)->GetTower());
//             grid.map[eventData.position.x][eventData.position.y].towers.push_back(level.GetObj<Platform>(eventData.position)->GetTower());
//             // std::cout << " tower: " << towers.size() << " grid.towers " << grid.map[eventData.position.x][eventData.position.y].towers.size();
//             break;
//         }
//         default: break;
//     }
// };

// void LevelManager::Update()
// {
//     for (auto& platform : level.GetPlatforms())
//     {
//         platform->Update();
//     }

//     HandleWave();

//     // for (auto& enemy : enemies)
//     // {
//     //     enemy->Update();
//     // }

//     grid.Update();
// };

// void LevelManager::Draw()
// {
//     for (auto& road : level.GetRoads())
//     {
//         road->Draw();
//     }
//     for (auto& platform : level.GetPlatforms())
//     {
//         platform->Draw();
//     }
//     level.GetBase()->Draw();
//     level.GetSpawnPoint()->Draw();

//     grid.Draw();
//     // for (auto& enemy : enemies)
//     // {
//     //     enemy->Draw();
//     // }
// };

// // void LevelManager::Level::InitializeMap()
// // {
// //     // clear previous map
// //     for (int x = 0; x < map.size(); ++x)
// //     {
// //         for (int y = 0; y < map[x].size(); ++y)
// //         {
// //             Vector2 currentPosition{static_cast<float>(x), static_cast<float>(y)};
// //             const Item& item = map[x][y].item;

// //             switch (item)
// //             {
// //                 case Item::BASE:
// //                 {
// //                     std::shared_ptr<Base> newBase = std::make_shared<Base>(currentPosition);
// //                     map[x][y].obj = newBase;
// //                     base = newBase;
// //                     levelManager->grid.map[x][y].terrain = newBase;
// //                     levelManager->grid.map[x][y].terrainId = Grid::Cell::BASE;
// //                     break;
// //                 }
// //                 case Item::ROAD:
// //                 {
// //                     std::shared_ptr<Road> road = std::make_shared<Road>(currentPosition);
// //                     map[x][y].obj = road;
// //                     roads.push_back(road);

// //                     levelManager->grid.map[x][y].terrain = road;
// //                     levelManager->grid.map[x][y].terrainId = Grid::Cell::ROAD;
// //                     break;
// //                 }
// //                 case Item::PLATFORM:
// //                 {
// //                     std::shared_ptr<Platform> platform = std::make_shared<Platform>(currentPosition);
// //                     platform->signal.Connect(levelManager->listener);
// //                     map[x][y].obj = platform;
// //                     platforms.push_back(platform);

// //                     levelManager->grid.map[x][y].terrain = platform;
// //                     levelManager->grid.map[x][y].terrainId = Grid::Cell::PLATFORM;
// //                     break;
// //                 }
// //                 case Item::SPAWN_POINT:
// //                 {
// //                     std::shared_ptr<SpawnPoint> newSpawn = std::make_shared<SpawnPoint>(currentPosition, levelManager->enemies, levelManager->grid);
// //                     map[x][y].obj = newSpawn;
// //                     spawnPoint = newSpawn;

// //                     levelManager->grid.map[x][y].terrain = newSpawn;
// //                     levelManager->grid.map[x][y].terrainId = Grid::Cell::SPAWN;
// //                     break;
// //                 }
// //             }
// //         }
// //     }
// // };

// // void LevelManager::Level::SetMap(GridMap& newMap)
// // {
// //     map = newMap;
// //     InitializeMap();
// // };
// // GridMap& LevelManager::Level::GetMap() { return map; };

// // template<typename T>
// // std::shared_ptr<T> LevelManager::Level::GetObj(int x, int y) { return std::get<std::shared_ptr<T>>(map[x][y].obj); };
// // template<typename T>
// // std::shared_ptr<T> LevelManager::Level::GetObj(Vector2 v) { return std::get<std::shared_ptr<T>>(map[v.x][v.y].obj); };
// // template<typename T>
// // std::shared_ptr<T> LevelManager::Level::GetObj()
// // {
// //     for (const auto& row : map)
// //     {
// //         for (const auto& cell : row)
// //         {
// //             if (auto obj = std::get<std::shared_ptr<T>>(cell.obj))
// //             {
// //                 return obj;
// //             }
// //         }
// //     }
// // };

// // std::shared_ptr<SpawnPoint> LevelManager::Level::GetSpawnPoint() { return spawnPoint; };
// // std::shared_ptr<Base> LevelManager::Level::GetBase() { return base; };
// // std::vector<std::shared_ptr<Road>> LevelManager::Level::GetRoads() { return roads; };
// // std::vector<std::shared_ptr<Platform>> LevelManager::Level::GetPlatforms() { return platforms; }
