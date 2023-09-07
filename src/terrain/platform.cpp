#include "platform.hpp"

#include "grid_helper.hpp"
#include "ui.hpp"

#include "event_list.hpp"

void Platform::Draw()
{
    GH::DrawRec(GetRec(), color);

    if (isActive)
    {
        if (tower)
        {
            tower->StatusPanel();
            UI::SidePanelCloseButton(isActive);
        }
        else
            DrawSelectTowerPanel();
    }

    if (tower)
    {
        tower->Draw();
    }
};

void Platform::DrawSelectTowerPanel()
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
        event->Emit(towerAdded);
    }
};

std::shared_ptr<Tower> Platform::GetTower()
{
    return tower;
};

bool Platform::HasTower()
{
    if (tower)
        return true;
    return false;
};

void Platform::Activate()
{
    isActive = true;
};
void Platform::Deactivate()
{
    isActive = false;
};

void Platform::ActivateOnClick()
{
    if (GH::ClickGrid(position.grid))
    {
        Activate();

        Event::PlatformActivated platformActivated;
        platformActivated.position.grid = position.grid;
        event->Emit(platformActivated);
    }
};
