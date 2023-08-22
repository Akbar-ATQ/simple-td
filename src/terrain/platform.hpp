#ifndef PLATFORM_HPP
#define PLATFORM_HPP

#include "global_data.hpp"
#include "tile.hpp"
#include "entity_data.hpp"
#include "tower.hpp"
#include "ui.hpp"

#include "event_signal.hpp"
#include "event_list.hpp"

#include "raylib.h"

#include <vector>
#include <memory>

class Platform
{
public:
    Platform(Vector2 pos) : data{pos, TILE_SIZE, BLACK} {};
    ~Platform() = default;

    EntityData data;
    std::shared_ptr<Event::Manager> eventEmitter = Event::Manager::Create();

    void Draw()
    {
        Tile::DrawRec(data.GetRec(), data.color);

        if (isActive)
        {
            if (tower)
            {
                tower->StatusPanel();
                UI::SidePanelCloseButton(isActive);
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
        UI::SidePanelCloseButton(isActive);

        // DrawText(TextFormat("Coins: %i", towerPrice), tower.data.position.x * TILE_SIZE, ((tower.data.position.y + 1) * TILE_SIZE), 20, BLACK);

        if (Tile::ClickTile(towerPanel.data.position))
        {
            tower = std::make_shared<Tower>(data.position);

            Event::TowerAdded towerAdded;
            towerAdded.position = data.position;
            eventEmitter->Emit(towerAdded);
        }
    };

    std::shared_ptr<Tower> GetTower() { return tower; };

    void Activate() { isActive = true; };
    void Deactivate() { isActive = false; };

    void ActivateOnClick()
    {
        if (Tile::ClickTile(data.position))
        {
            Activate();

            Event::PlatformActivated platformActivated;
            platformActivated.position = data.position;
            eventEmitter->Emit(platformActivated);
        }
    };

    void Update()
    {
        ActivateOnClick();
        if (tower) tower->Update();
    };

private:
    bool isActive {false};

    std::shared_ptr<Tower> tower;
};

#endif