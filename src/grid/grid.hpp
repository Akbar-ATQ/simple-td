#ifndef GRID_HPP
#define GRID_HPP

#include "global_data.hpp"
#include "enemy.hpp"
#include "tower.hpp"
#include "platform.hpp"
#include "base.hpp"
#include "road.hpp"
#include "spawner.hpp"

#include <vector>
#include <memory>
#include <variant>

class Grid
{
public:
    Grid(int x, int y) : position{x, y} {};
    ~Grid() = default;

    const Vec2i position;

    // ---------- Entity ---------- //

    TerrainID terrainId {TerrainID::EMPTY};

    std::variant<
        std::shared_ptr<Base>,
        std::shared_ptr<Road>,
        std::shared_ptr<Spawner>,
        std::shared_ptr<Platform>
    > terrain;

    std::vector<std::weak_ptr<Enemy>> enemies;
    std::vector<std::shared_ptr<Tower::Bullet>> bullets;

    // ------------------------------ //

    template<typename T>
    std::shared_ptr<T> GetTerrain()
    {
        return std::get<std::shared_ptr<T>>(terrain);
    };

    bool IsEmpty()
    {
        if (terrainId == TerrainID::EMPTY && (enemies.empty() && bullets.empty()))
            return true;
        return false;
    };
};

#endif