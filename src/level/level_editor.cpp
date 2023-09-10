#include "level_editor.hpp"

#include <fstream>

void Level::Editor::DrawDummyTerrain(const int x, const int y)
{
    Rectangle dummy;
    Color color;

    switch (levelData[x][y])
    {
        case TerrainID::ROAD:
        {
            dummy = {static_cast<float>(x * GRID_SIZE), static_cast<float>(y * GRID_SIZE), dummyRoad.size, dummyRoad.size};
            color = dummyRoad.color;
            break;
        }
        case TerrainID::BASE:
        {
            dummy = {static_cast<float>(x * GRID_SIZE), static_cast<float>(y * GRID_SIZE), dummyBase.size, dummyBase.size};
            color = dummyBase.color;
            break;
        }
        case TerrainID::SPAWNER:
        {
            dummy = {static_cast<float>(x * GRID_SIZE), static_cast<float>(y * GRID_SIZE), dummySpawner.size, dummySpawner.size};
            color = dummySpawner.color;
            break;
        }
        case TerrainID::PLATFORM:
        {
            dummy = {static_cast<float>(x * GRID_SIZE), static_cast<float>(y * GRID_SIZE), dummyPlatform.size, dummyPlatform.size};
            color = dummyPlatform.color;
            break;
        }
    }

    GH::DrawRec(dummy, color);
};

void Level::Editor::EditLevel()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        selectedGrid = GH::GridPosition(GetMousePosition());

        levelData[selectedGrid.x][selectedGrid.y] = terrain;
    }

    if (IsKeyPressed(KEY_ZERO))
        terrain = TerrainID::EMPTY;
    else if (IsKeyPressed(KEY_ONE))
        terrain = TerrainID::ROAD;
    else if (IsKeyPressed(KEY_TWO))
        terrain = TerrainID::BASE;
    else if (IsKeyPressed(KEY_THREE))
        terrain = TerrainID::SPAWNER;
    else if (IsKeyPressed(KEY_FOUR))
        terrain = TerrainID::PLATFORM;

    BeginDrawing();

        ClearBackground(RAYWHITE);

        GH::DrawGrid();

        for (int x = 0; x < levelData.size(); ++x)
        {
            for (int y = 0; y < levelData[x].size(); ++y)
            {
                if (levelData[x][y] != TerrainID::EMPTY)
                {
                    DrawDummyTerrain(x, y);
                }
            }
        }

    EndDrawing();
};

void Level::Editor::SaveLevel()
{
    std::ofstream levelFile("../assets/level/custom_level.map", std::ios::binary);
    if (levelFile.is_open())
    {
        for (int x = 0; x < levelData.size(); ++x)
        {
            for (int y = 0; y < levelData[x].size(); ++y)
            {
                levelFile << static_cast<int>(levelData[x][y]);
            }
            levelFile << "\n";
        }

        levelFile.close();
    }
};

void Level::Editor::Update()
{
    EditLevel();
    if (IsKeyPressed(KEY_S))
        SaveLevel();
};
