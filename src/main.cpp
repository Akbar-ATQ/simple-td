#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "level_manager.hpp"
#include "level_loader.hpp"
#include "level_editor.hpp"
#include "grid_helper.hpp"
#include "global_data.hpp"

#include "raylib.h"

int main()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple TD V2");

    LevelData level1Map;
    Level::Loader("../assets/level/custom_level.map", level1Map);
    Level::Manager level;
    level.GenerateLevel(level1Map);

    bool editMode {false};

    Level::Editor levelEditor;

    // Main game loop
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        if (IsKeyPressed(KEY_E))
            editMode = !editMode;

        if (editMode)
        {
            levelEditor.Update();
        }
        else
        {
            level.Update();

            BeginDrawing();

                ClearBackground(RAYWHITE);

                GH::DrawGrid();

                level.Draw();

            EndDrawing();
        }
    }

    // Cleanup
    CloseWindow();

    return 0;
}
