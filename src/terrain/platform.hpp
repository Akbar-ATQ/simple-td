#ifndef TERRAIN_PLATFORM_HPP
#define TERRAIN_PLATFORM_HPP

#include "global_data.hpp"
#include "entity.hpp"
#include "tower.hpp"

#include "event_signal.hpp"

#include "raylib.h"

#include <vector>
#include <memory>

class Platform : public Entity
{
public:
    Platform(Vec2i gridPos) : Entity{Position{gridPos, {0, 0}}, GRID_SIZE, BLACK} {};
    ~Platform() = default;

    std::shared_ptr<Event::Manager> event = Event::Manager::Create();

    void Draw();

    void DrawSelectTowerPanel();

    std::shared_ptr<Tower> GetTower();
    bool HasTower();

    void Activate();
    void Deactivate();

    void ActivateOnClick();

private:
    bool isActive {false};

    std::shared_ptr<Tower> tower;
};

#endif