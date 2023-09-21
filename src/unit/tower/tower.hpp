#ifndef UNIT_TOWER_HPP
#define UNIT_TOWER_HPP

#include "entity.hpp"
#include "global_data.hpp"
#include "enemy.hpp"
#include "bullet.hpp"
#include "event_signal.hpp"
#include "event_list.hpp"

#include "raylib.h"

#include <vector>
#include <memory>

class Tower : public Entity
{
public:
    Tower(Vec2i gridPos, Vec2f localPos) : Entity{Position{gridPos, localPos}, GRID_SIZE, GREEN}
    {
        listener = Event::Signal::Create(this);
        listener->Connect<Event::HitEnemy, Tower>(Event::HitEnemy(), &Tower::OnHitEnemy);
    };
    ~Tower() = default;

    const Vec2f center {GRID_SIZE / 2, GRID_SIZE / 2};

    enum class Stat
    {
        LEVEL,
        EXP,
        DMG,
        RANGE,
        ATK_SPD,
        PROJECTILE_SPD,
    };

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

    std::shared_ptr<Event::Signal> listener;
    void OnHitEnemy(Event::HitEnemy hitEnemy);

    void LevelUp();

    void Upgrade(Stat stat, float &value, int order, int textSize);

    void DrawStat(std::string text, const float value, int &order, const int textSize);
    void StatusPanel();

    std::vector<std::shared_ptr<Bullet>> GetBullets();

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

    int level {1};
    int exp {0};
    int nextLevelExp {100};

    int upgradeNum {0};
};

#endif