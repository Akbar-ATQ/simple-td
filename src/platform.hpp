#ifndef PLATFORM_H
#define PLATFORM_H

#include "global_data.hpp"
#include "tile.hpp"
#include "entity_data.hpp"
#include "tower.hpp"

#include "signal.hpp"

#include "raylib.h"
#include "raygui.h"

#include <vector>
// #include <memory>
#include <iostream>

class Platform
{
public:
    Platform(Vector2 pos) : data{pos, TILE_SIZE, BLACK} {};
    ~Platform() = default;

    EntityData data;
    Signal::Signal signal;
    bool isSelectingTower {false};

    void Draw()
    {
        Tile::DrawRec(data.GetRec(), data.color);

        if (isSelectingTower)
        {
            DrawSelectTowerPanel();
        }
    };

    void DrawSelectTowerPanel()
    {
        float width {3.0f};
        Rectangle sidePanel {(MAP_SIZE.x - width), 0, (TILE_SIZE * width), SCREEN_HEIGHT};

        Tile::DrawRec(sidePanel, BROWN);

        Tower tower {(MAP_SIZE.x - (width - 1)), 2, GREEN};
        // int towerPrice {10};

        tower.Draw();
        Rectangle closeButton {(sidePanel.x + 2) * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
        if (GuiButton(closeButton, "Close")) isSelectingTower = false;

        // DrawText(TextFormat("Coins: %i", towerPrice), tower.data.position.x * TILE_SIZE, ((tower.data.position.y + 1) * TILE_SIZE), 20, BLACK);

        if (Tile::ClickTile(tower.data.position)) signal.Notify({Signal::Event::ADD_TOWER, data.position});
    };

    void SelectTower()
    {
        // Send signal to levelManager {Event::SELECTING_TOWER, this platform position}
        // levelManager then search platform in EventData.position and turn `isSelectingTower` true and other platform false
        // then in Draw() if (isSelectingTower) DrawSelectTowerPanel()
        if (Tile::ClickTile(data.position)) signal.Notify({Signal::Event::SELECTING_TOWER, data.position});
    };

    void Update()
    {
        SelectTower();
    };

private:
};

#endif