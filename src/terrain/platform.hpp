#ifndef TERRAIN_PLATFORM_HPP
#define TERRAIN_PLATFORM_HPP

#include "grid_helper.hpp"
#include "global_data.hpp"
#include "entity.hpp"
#include "tower.hpp"
#include "ui.hpp"

#include "event_signal.hpp"
#include "event_list.hpp"

#include "raylib.h"

#include <vector>
#include <memory>

class Platform : public Entity
{
public:
    Platform(Vec2i gridPos) : Entity{Position{gridPos, {0, 0}}, GRID_SIZE, BLACK} {};
    ~Platform() = default;

    // EntityData data;
    std::shared_ptr<Event::Manager> eventEmitter = Event::Manager::Create();

    void Draw()
    {
        GH::DrawRec(GetRec(), color);

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

        static Tower towerPanel {Vec2i{(MAP_SIZE.x - (static_cast<int>(UI::sidePanelWidth) - 1)), 2}, {0, 0}};
        // int towerPrice {10};

        towerPanel.Draw();
        UI::SidePanelCloseButton(isActive);

        // DrawText(TextFormat("Coins: %i", towerPrice), tower.data.position.x * TILE_SIZE, ((tower.data.position.y + 1) * TILE_SIZE), 20, BLACK);

        if (GH::ClickGrid(towerPanel.position.grid))
        {
            tower = std::make_shared<Tower>(position.grid, position.local);

            Event::TowerAdded towerAdded;
            towerAdded.position.grid = position.grid;
            eventEmitter->Emit(towerAdded);
        }
    };

    std::shared_ptr<Tower> GetTower() { return tower; };
    bool HasTower()
    {
        if (tower) return true;
        return false;
    };

    void Activate() { isActive = true; };
    void Deactivate() { isActive = false; };

    void ActivateOnClick()
    {
        if (GH::ClickGrid(position.grid))
        {
            Activate();

            Event::PlatformActivated platformActivated;
            platformActivated.position.grid = position.grid;
            eventEmitter->Emit(platformActivated);
        }
    };

    void Update()
    {
        ActivateOnClick();
    };

private:
    bool isActive {false};

    std::shared_ptr<Tower> tower;
};

#endif