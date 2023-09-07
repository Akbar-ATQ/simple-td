#ifndef UNIT_TOWER_HPP
#define UNIT_TOWER_HPP

#include "grid_helper.hpp"
#include "entity.hpp"
#include "global_data.hpp"
#include "ui.hpp"
#include "enemy.hpp"
#include "bullet.hpp"

#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <memory>

class Tower : public Entity
{
public:
    Tower(Vec2i gridPos, Vec2f localPos) : Entity{Position{gridPos, localPos}, GRID_SIZE, GREEN} {};
    ~Tower() = default;

    const Vec2f center {GRID_SIZE / 2, GRID_SIZE / 2};

    // ---------- Stats ---------- //
    float detectionRange {4};
    float attackTimer {0.0f};
    float attackSpeed {0.2f};
    float projectileSpeed {160.0f};
    float bulletDamage {1};
    // --------------------------- //

    void DrawDetectionRange();
    void Draw()
    {
        GH::DrawRec(GetRec(), color);
    };

    void Update()
    {
        // Nothing to update here.
    };

    void SelectTarget(std::vector<std::shared_ptr<Enemy>> enemiesInRange)
    {
        // for (const auto& enemy : enemiesInRange)
        // {
        //     /* code to select target */
        // }

        if (!enemiesInRange.empty())
        {
            target.enemy = enemiesInRange[0];
            target.isExist = true;
        }
        else
            target.isExist = false;
    };
    void Shoot(std::vector<std::shared_ptr<Enemy>> enemiesInRange)
    {
        SelectTarget(enemiesInRange);

        if (target.isExist)
        {
            attackTimer += GetFrameTime();
            if (attackTimer >= attackSpeed)
            {
                attackTimer = 0.0f;

                std::shared_ptr<Bullet> bullet = Bullet::Create(position.grid, center, projectileSpeed, bulletDamage, target.enemy);

                bullets.push_back(bullet);
            }
        }
    };

    std::vector<std::shared_ptr<Bullet>> GetBullets() { return bullets; };

    void Upgrade();

    void DrawStat(std::string text, float stat)
    {
        DrawText(TextFormat("%s: %.3f", text.c_str(), stat),
            GH::RealPosition(UI::sidePanelPos.x),
            GH::RealPosition(UI::sidePanelPos.y + 2),
            14,
            BLACK
        );
    }
    void StatusPanel()
    {
        UI::SidePanel();
        DrawStat("Damage", bulletDamage);
    };

    void MoveBullets(std::vector<std::shared_ptr<Bullet>> &otherBullets)
    {
        otherBullets.insert(otherBullets.end(), std::make_move_iterator(bullets.begin()), std::make_move_iterator(bullets.end()));
        bullets.clear();
    };

private:
    struct Target
    {
        bool isExist {false};
        std::weak_ptr<Enemy> enemy;
    };
    Target target;

    std::vector<std::shared_ptr<Bullet>> bullets;
};

#endif