#ifndef PLATFORM_H
#define PLATFORM_H

#include "global_data.hpp"
#include "tile.hpp"
#include "entity_data.hpp"
#include "tower.hpp"
#include "ui.hpp"

#include "signal.hpp"

#include "raylib.h"

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
                tower->StatusPanel();
                UI::CloseSidePanelButton(isActive);
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
        UI::SidePanel();

        static Tower towerPanel {Vector2{(MAP_SIZE.x - (UI::sidePanelWidth - 1)), 2}};
        // int towerPrice {10};

        towerPanel.Draw();
        UI::CloseSidePanelButton(isActive);

        // DrawText(TextFormat("Coins: %i", towerPrice), tower.data.position.x * TILE_SIZE, ((tower.data.position.y + 1) * TILE_SIZE), 20, BLACK);

        if (Tile::ClickTile(towerPanel.data.position))
        {
            tower = std::make_shared<Tower>(data.position);
        }
    };

    void Activate() { isActive = true; };
    void Deactivate() { isActive = false; };

    void SelectTower()
    {
        // Send signal to levelManager {Event::SELECTING_TOWER, this platform position}
        // levelManager then search platform in EventData.position and turn `isActive` true and other platform false
        // then in Draw() if (isActive) DrawSelectTowerPanel()
        if (Tile::ClickTile(data.position)) signal.Notify({Signal::Event::ACTIVATING_PLATFORM, data.position});
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