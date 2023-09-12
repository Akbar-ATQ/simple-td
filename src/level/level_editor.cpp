#include "level_editor.hpp"

#include "ui.hpp"

#include "raygui.h"

#include <fstream>
#include <string>
#include <cstring>

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
};

void Level::Editor::SaveLevel()
{
    GuiTextBox(textBox, levelName, MAX_CHARS, true);

    bool saveLevel {false};

    Vec2i yesButtonPos {static_cast<int>(textBox.x / GRID_SIZE), static_cast<int>((textBox.y / GRID_SIZE) + 1)};
    Rectangle yesButtonRec {
        static_cast<float>(yesButtonPos.x * GRID_SIZE),
        static_cast<float>(yesButtonPos.y * GRID_SIZE),
        (2 * GRID_SIZE),
        GRID_SIZE
    };
    if (GuiButton(yesButtonRec, "YES"))
        saveLevel = true;

    Vec2i noButtonPos {(yesButtonPos.x + 4), yesButtonPos.y};
    Rectangle noButtonRec {
        static_cast<float>(noButtonPos.x * GRID_SIZE),
        static_cast<float>(noButtonPos.y * GRID_SIZE),
        (2 * GRID_SIZE),
        GRID_SIZE
    };
    if (GuiButton(noButtonRec, "NO"))
    {
        isSaving = false;
        std::strcpy(levelName, "");
    }

    if (IsKeyPressed(KEY_ENTER) || saveLevel)
    {
        std::string path {"../assets/level/"};
        path.append(levelName);
        path.append(".map");

        std::ofstream levelFile(path, std::ios::binary);

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

        isSaving = false;
        std::strcpy(levelName, "");
    }
};

void Level::Editor::SidePanel()
{
    UI::SidePanel();

    Vec2i sidePanelPos {(MAP_SIZE.x - (static_cast<int>(UI::sidePanelWidth))), 0};

    Vec2i roadPos {(sidePanelPos.x + 1), 2};
    Rectangle road {GH::RealPosition(roadPos.x), GH::RealPosition(roadPos.y), dummyRoad.size, dummyRoad.size};
    GH::DrawRec(road, dummyRoad.color);

    Vec2i basePos {(sidePanelPos.x + 1), 4};
    Rectangle base {GH::RealPosition(basePos.x), GH::RealPosition(basePos.y), dummyBase.size, dummyBase.size};
    GH::DrawRec(base, dummyBase.color);

    Vec2i spawnerPos {(sidePanelPos.x + 1), 6};
    Rectangle spawner {GH::RealPosition(spawnerPos.x), GH::RealPosition(spawnerPos.y), dummySpawner.size, dummySpawner.size};
    GH::DrawRec(spawner, dummySpawner.color);

    Vec2i platformPos {(sidePanelPos.x + 1), 8};
    Rectangle platform {GH::RealPosition(platformPos.x), GH::RealPosition(platformPos.y), dummyPlatform.size, dummyPlatform.size};
    GH::DrawRec(platform, dummyPlatform.color);

    Vec2i emptyPos {(sidePanelPos.x + 1), 10};
    Rectangle empty {GH::RealPosition(emptyPos.x), GH::RealPosition(emptyPos.y), GRID_SIZE, GRID_SIZE};
    GH::DrawRec(empty, RAYWHITE);

    if (GH::ClickGrid(roadPos))
        terrain = TerrainID::ROAD;
    else if (GH::ClickGrid(basePos))
        terrain = TerrainID::BASE;
    else if (GH::ClickGrid(spawnerPos))
        terrain = TerrainID::SPAWNER;
    else if (GH::ClickGrid(platformPos))
        terrain = TerrainID::PLATFORM;
    else if (GH::ClickGrid(emptyPos))
        terrain = TerrainID::EMPTY;

    Rectangle saveButton {GH::RealPosition(sidePanelPos.x), GH::RealPosition(13), (3 * GRID_SIZE), GRID_SIZE};

    if (GuiButton(saveButton, "SAVE"))
        isSaving = true;
};

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

void Level::Editor::Update()
{
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

        SidePanel();

        if (isSaving)
            SaveLevel();
        else
            EditLevel();

    EndDrawing();
};
