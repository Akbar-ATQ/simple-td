#ifndef UNIT_TOWER_HPP
#define UNIT_TOWER_HPP

#include "entity.hpp"
#include "global_data.hpp"
#include "enemy.hpp"
#include "bullet.hpp"

#include "raylib.h"

#include <vector>
#include <memory>

class Tower : public Entity
{
public:
    Tower(Vec2i gridPos, Vec2f localPos) : Entity{Position{gridPos, localPos}, GRID_SIZE, GREEN} {};
    ~Tower() = default;

    const Vec2f center {GRID_SIZE / 2, GRID_SIZE / 2};

    // ---------- Stats ---------- //
    float bulletDamage {1};
    float detectionRange {4};
    float attackSpeed {0.2f};
    float projectileSpeed {160.0f};
    // --------------------------- //

    void DrawDetectionRange();
    void Draw();

    void SelectTarget(std::vector<std::shared_ptr<Enemy>> enemiesInRange);
    void Shoot(std::vector<std::shared_ptr<Enemy>> enemiesInRange);

    std::vector<std::shared_ptr<Bullet>> GetBullets();

    void Upgrade();

    void DrawStat(std::string text, float stat, int &order);
    void StatusPanel();

    void MoveBullets(std::vector<std::shared_ptr<Bullet>> &otherBullets);

private:
    struct Target
    {
        bool isExist {false};
        std::weak_ptr<Enemy> enemy;
    };
    Target target;

    std::vector<std::shared_ptr<Bullet>> bullets;

    float attackTimer {0.0f};
};

#endif