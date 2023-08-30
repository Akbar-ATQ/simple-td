#define RAYGUI_IMPLEMENTATION

#include "tile.hpp"
#include "global_data.hpp"
#include "level.hpp"

#include "raylib.h"

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

    Vector2 base {2, 5};
    Vector2 spawner {2, 2};

    testLevel[base.x][base.y] = TerrainID::BASE;
    testLevel[spawner.x][spawner.y] = TerrainID::SPAWNER;

    for (int x = spawner.x + 1; x < 10; ++x)
    {
        testLevel[x][spawner.y] = TerrainID::ROAD;
    }
    for (int y = spawner.y; y < 8; ++y)
    {
        testLevel[10][y] = TerrainID::ROAD;
    }
    for (int x = base.x; x < 11; ++x)
    {
        testLevel[x][8] = TerrainID::ROAD;
    }
    for (int y = base.y + 1; y < 8; ++y)
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
    Vector2 platform3 {6, 6};

    testLevel[platform1.x][platform1.y] = TerrainID::PLATFORM;
    testLevel[platform2.x][platform2.y] = TerrainID::PLATFORM;
    testLevel[platform3.x][platform3.y] = TerrainID::PLATFORM;

    return testLevel;
};

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple TD V2");

    LevelData testLevel = TestLevel();
    Level level;
    level.GenerateLevel(testLevel);

    // Main game loop
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        level.Update();

        BeginDrawing();

            ClearBackground(RAYWHITE);

            Tile::Draw();

            level.Draw();

        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}
