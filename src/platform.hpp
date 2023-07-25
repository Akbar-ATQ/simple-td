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
#include <memory>

class Platform
{
public:
    Platform(Vector2 pos) : data{pos, TILE_SIZE, BLACK} {};
    ~Platform() = default;

    EntityData data;
    Signal::Signal signal;

    void Draw()
    {
        Tile::DrawRec(data.GetRec(), data.color);

        if (isActive)
        {
            if (tower)
            {
                float width {3.0f};
                Rectangle sidePanel {(MAP_SIZE.x - width), 0, (TILE_SIZE * width), SCREEN_HEIGHT};
                tower->StatusPanel();
                Rectangle closeButton {(sidePanel.x + 2) * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
                if (GuiButton(closeButton, "Close")) Deactive();
            }
            else DrawSelectTowerPanel();
        }

        if (tower)
        {
            tower->Draw();
        }
    };

    void DrawSelectTowerPanel()
    {
        float width {3.0f};
        Rectangle sidePanel {(MAP_SIZE.x - width), 0, (TILE_SIZE * width), SCREEN_HEIGHT};

        Tile::DrawRec(sidePanel, BROWN);

        Tower towerPanel {Vector2{(MAP_SIZE.x - (width - 1)), 2}};
        // int towerPrice {10};

        towerPanel.Draw();
        Rectangle closeButton {(sidePanel.x + 2) * TILE_SIZE, 0, TILE_SIZE, TILE_SIZE};
        if (GuiButton(closeButton, "Close")) Deactive();

        // DrawText(TextFormat("Coins: %i", towerPrice), tower.data.position.x * TILE_SIZE, ((tower.data.position.y + 1) * TILE_SIZE), 20, BLACK);

        if (Tile::ClickTile(towerPanel.data.position))
        {
            tower = std::make_shared<Tower>(data.position);
        }
    };

    void Activating() { isActive = true; };
    void Deactive() { isActive = false; };

    void SelectTower()
    {
        // Send signal to levelManager {Event::SELECTING_TOWER, this platform position}
        // levelManager then search platform in EventData.position and turn `isActive` true and other platform false
        // then in Draw() if (isActive) DrawSelectTowerPanel()
        if (Tile::ClickTile(data.position)) signal.Notify({Signal::Event::ACTIVATING_TOWER, data.position});
    };

    void Update()
    {
        SelectTower();
    };

private:
    bool isActive {false};

    std::shared_ptr<Tower> tower;
};

#endif