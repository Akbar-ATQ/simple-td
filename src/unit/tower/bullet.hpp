#ifndef UNIT_BULLET_HPP
#define UNIT_BULLET_HPP

#include "entity.hpp"
#include "enemy.hpp"
#include "global_data.hpp"

#include <memory>

class Bullet : public Entity
{
public:
    Bullet() = default;
    ~Bullet() = default;

    Position prevPosition;

    std::weak_ptr<Enemy> enemy;

    // ---------- stats ---------- //
    float speed {0};
    Vec2f direction {0, 0};
    float damage {0};
    // ---------------------------- //

    // std::shared_ptr<Event::Signal> hittingEnemy // When hitting enemy, take exp and give it to tower

    void Draw();

    void Move();

    static std::shared_ptr<Bullet> Create(
        const Vec2i &grid,
        const Vec2f &local,
        const float &speed,
        const float &damage,
        const std::weak_ptr<Enemy> &enemy
    );
};

#endif