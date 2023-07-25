#ifndef TOWER_HPP
#define TOWER_HPP

// #include "enemy.h"
#include "entity_data.hpp"
#include "global_data.hpp"

#include "raylib.h"

#include <vector>

class Tower
{
public:
    Tower(float x, float y, Color color) : data{{x, y}, color} {};
    ~Tower() = default;

    struct Data : public EntityData
    {
        const Vector2 center;

        // ---------- Stats ---------- //
        float detectionRadius {4};
        float attackTimer {0.0f};
        float attackSpeed {0.2f};
        float bulletSpeed {2.0f};
        float bulletDamage {1};

        Data(Vector2 pos, Color color) : EntityData{pos, TILE_SIZE, color}, center{pos.x + 0.5f, pos.y + 0.5f} {};
    };

    Data data;

    enum class Target
    {
        FIRST,
        LAST,
        NEAR,
        STRONG,
        WEAK,
    };

    void DrawDetectionRange();
    void Draw() { Tile::DrawRec(data.GetRec(), data.color); };

    void Update();

    void Shoot();
    void DrawBullets();
    int GetBullets() { return m_bullets.size(); };

    void Upgrade();

private:
    struct Bullet {
        Vector2 pos;
        Vector2 prevPos;
        float radius;
        float speed;
        Vector2 velocity {0, 0};
        Vector2 target;
        int damage;
    };

    std::vector<Bullet> m_bullets;

    Target target {Target::FIRST};
};

#endif