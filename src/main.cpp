#define RAYGUI_IMPLEMENTATION
#include "tile.hpp"
#include "global_data.hpp"
#include "level_manager.hpp"

#include "raylib.h"

#include <iostream>

LevelData TestLevel()
{
    LevelData testLevel;

    for (int x = 0; x < MAP_SIZE.x; ++x)
    {
        std::vector<TerrainID> levelY;

        for (int y = 0; y < MAP_SIZE.y; ++y)
        {
            levelY.push_back(TerrainID::EMPTY);
        }

        testLevel.push_back(levelY);
    }

    Vector2 base {10, 8};
    Vector2 spawner {2, 2};

    testLevel[base.x][base.y] = TerrainID::BASE;
    testLevel[spawner.x][spawner.y] = TerrainID::SPAWNER;

    for (int x = spawner.x + 1; x < base.x; ++x)
    {
        testLevel[x][spawner.y] = TerrainID::ROAD;
    }
    for (int y = spawner.y; y < base.y; ++y)
    {
        testLevel[base.x][y] = TerrainID::ROAD;
    }

    // branch road
    for (int i = 0; i < 5; ++i)
    {
        testLevel[5][spawner.y + i] = TerrainID::ROAD;
    }

    Vector2 platform1 {8, 4};
    Vector2 platform2 {8, 1};

    testLevel[platform1.x][platform1.y] = TerrainID::PLATFORM;
    testLevel[platform2.x][platform2.y] = TerrainID::PLATFORM;

    return testLevel;
};

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple TD V2");

    LevelData testLevel = TestLevel();
    // LevelManager level;
    // level.GenerateLevel(testLevel);

    // Main game loop
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        // level.Update();

        BeginDrawing();

            ClearBackground(RAYWHITE);

            Tile::Draw();

            // level.Draw();

            // if (Tile::ClickTile({0, 0})) std::cout << "clicked ";

            // testTower.Draw();

        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}
