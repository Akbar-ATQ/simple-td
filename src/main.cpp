#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "level_manager.hpp"
#include "level_loader.hpp"
#include "level_editor.hpp"
#include "grid_helper.hpp"
#include "global_data.hpp"

#include "raylib.h"

#include <string>

Mode mode {MAIN_MENU};

void SelectLevel(int &order, bool &isLoadLevel, std::string &levelName, std::string name)
{
    Rectangle levelButton {
        static_cast<float>(8 * GRID_SIZE),
        static_cast<float>(order * GRID_SIZE),
        static_cast<float>(4 * GRID_SIZE),
        static_cast<float>(GRID_SIZE)
    };

    if (GuiButton(levelButton, name.c_str()))
    {
        levelName = name;
        isLoadLevel = true;
    }

    order += 1;
};

void MainMenu(Level::Manager &level, std::string &levelPath)
{
    bool isLoadLevel {false};
    std::string levelName;
    int order {2}; // Where the level button in y axis.

    SelectLevel(order, isLoadLevel, levelName, "LEVEL 1");
    SelectLevel(order, isLoadLevel, levelName, "LEVEL 2");

    if (isLoadLevel)
    {
        std::string levelFile;
        levelFile.append(levelPath);
        levelFile.append(levelName);
        levelFile.append(".map");

        LevelData levelData = Level::Loader(levelFile);
        level.GenerateLevel(levelData);

        mode = PLAY;
    }

    // ---------- Level Editor Button ---------- //
    Rectangle levelEditorButton {
        static_cast<float>(15 * GRID_SIZE),
        static_cast<float>(2 * GRID_SIZE),
        static_cast<float>(4 * GRID_SIZE),
        static_cast<float>(GRID_SIZE)
    };

    if (GuiButton(levelEditorButton, "Level Editor"))
    {
        mode = LEVEL_EDITOR;
    }
};

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple TD V2");

    Level::Manager level;
    std::string levelPath {"../assets/level/"};

    Level::Editor levelEditor;

    // Main game loop
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

        Rectangle menuButton {
                static_cast<float>(17 * GRID_SIZE),
                static_cast<float>(0 * GRID_SIZE),
                static_cast<float>(3 * GRID_SIZE),
                static_cast<float>(GRID_SIZE)
            };

        BeginDrawing();

        ClearBackground(RAYWHITE);

        GH::DrawGrid();

        if (mode == MAIN_MENU)
        {
            MainMenu(level, levelPath);
        }
        else if (mode == PLAY)
        {
            level.Update();

            level.Draw();

            if (GuiButton(menuButton, "Main Menu"))
            {
                level.Clear();
                mode = MAIN_MENU;
            }
        }
        else if (mode == LEVEL_EDITOR)
        {
            levelEditor.UpdateAndDraw();

            if (GuiButton(menuButton, "Main Menu"))
                mode = MAIN_MENU;
        }
        
        EndDrawing();
    }

    // Cleanup
    CloseWindow();

    return 0;
}
