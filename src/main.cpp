#define RAYGUI_IMPLEMENTATION
#include "tile.hpp"
#include "global_data.hpp"
#include "platform.hpp"
#include "level_manager.hpp"
#include "tower.hpp"

#include "raylib.h"

#include <vector>

#include <iostream>

GridMap Level1Map()
{
    std::vector<Vector2> road;
    GridMap level1map;

    Vector2 base {10, 8};
    Vector2 spawn {2, 2};

    for (int x = spawn.x + 1; x < base.x; ++x)
    {
        road.push_back({static_cast<float>(x), spawn.y});
    }
    for (int y = spawn.y; y < base.y; ++y)
    {
        road.push_back({base.x, static_cast<float>(y)});
    }
    for (int i = 0; i < 5; ++i)
    {
        road.push_back({5, spawn.y + i});
    }

    for (int x = 0; x < MAP_SIZE.x; ++x)
    {
        std::vector<MapData> levelY;
        for (int y = 0; y < MAP_SIZE.y; ++y)
        {
            if (x == base.x && y == base.y) levelY.push_back({static_cast<float>(x), static_cast<float>(y), Item::BASE});
            else if (x == spawn.x && y == spawn.y) levelY.push_back({static_cast<float>(x), static_cast<float>(y), Item::SPAWN_POINT});
            else levelY.push_back({static_cast<float>(x), static_cast<float>(y), Item::EMPTY});
        }

        level1map.push_back(levelY);
    }

    for (const auto& roadTile : road)
    {
        level1map[roadTile.x][roadTile.y].item = Item::ROAD;
    }

    level1map[8][4].item = Item::PLATFORM;
    level1map[8][1].item = Item::PLATFORM;

    return level1map;
};

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple TD V2");

    GridMap level1Map = Level1Map();
    LevelManager level;
    level.GenerateLevel(level1Map);

    // Tower testTower {platforms[0].data.position.x, platforms[0].data.position.y, GREEN};

    // Main game loop
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        level.Update();

        BeginDrawing();

            ClearBackground(RAYWHITE);

            Tile::Draw();

            level.Draw();

            // if (Tile::ClickTile({0, 0})) std::cout << "clicked ";

            // testTower.Draw();

        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}
