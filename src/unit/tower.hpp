#ifndef UNIT_TOWER_HPP
#define UNIT_TOWER_HPP

#include "grid_helper.hpp"
#include "entity.hpp"
#include "global_data.hpp"
#include "ui.hpp"
#include "enemy.hpp"

#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <memory>

class Tower : public Entity
{
public:
    Tower(Vec2i gridPos, Vec2f localPos)
    : Entity{Position{gridPos, localPos}, GRID_SIZE, GREEN},
    center{gridPos.Cast<float>().x + 0.5f, gridPos.Cast<float>().y + 0.5f}
    {};
    ~Tower() = default;

    // struct Data : public EntityData
    // {
    //     Data(Vec2i gridPos, Vec2f localPos, Color color)
    //     : EntityData{gridPos, localPos, TILE_SIZE, color},
    //     center{gridPos.Cast<float>().x + 0.5f, gridPos.Cast<float>().y + 0.5f}
    //     {};

    //     const Vec2f center;

    //     // ---------- Stats ---------- //
    //     float detectionRange {4};
    //     float attackTimer {0.0f};
    //     float attackSpeed {0.2f};
    //     float projectileSpeed {1.0f}; // 10.0 is the cap
    //     float bulletDamage {1};
    // };
    const Vec2f center;

    // ---------- Stats ---------- //
    float detectionRange {4};
    float attackTimer {0.0f};
    float attackSpeed {0.2f};
    float projectileSpeed {1.0f}; // 10.0 is the cap
    float maxProjectileSpeed {10.0f};
    float bulletDamage {1};

    struct Bullet
    {
        Bullet(Tower* tower) : tower{tower} {};

        Position position;
        Position prevPosition;

        float radius;
        float speed;
        Vec2f velocity {0, 0};
        Vec2f target;

        float damage;

        void Update()
        {
            // prevposition.grid = position.grid;
            // prevLocalPosition = position.local;
            // position.local += velocity * speed;
            // localPos.y += velocity.y * speed;
        };

        void RemoveFromTower(std::shared_ptr<Bullet> bullet) { tower->RemoveBullet(bullet); };
    private:
        Tower* tower;
    };

    // Data data;

    // enum class Target
    // {
    //     FIRST,
    //     LAST,
    //     NEAR,
    //     STRONG,
    //     WEAK,
    // };

    // Rectangle GetDetectionSqr() { return {}; };

    void DrawDetectionRange();
    void Draw()
    {
        GH::DrawRec(GetRec(), color);

        for (const auto& bullet : m_bullets)
        {
            GH::DrawCirc(GH::MergeReal(bullet->position.grid, bullet->position.local), bullet->radius, BLACK);
        }
    };

    void Update()
    {
        // if (!m_bullets.empty())
        // {
        //     for (auto bullet = m_bullets.begin(); bullet != m_bullets.end();)
        //     {
        //         (*bullet)->prevPos = (*bullet)->pos;
        //         (*bullet)->pos.x += (*bullet)->velocity.x * (*bullet)->speed;
        //         (*bullet)->pos.y += (*bullet)->velocity.y * (*bullet)->speed;

        //         if ((*bullet)->pos.x <= 0 || (*bullet)->pos.x >= MAP_SIZE.x
        //             || (*bullet)->pos.y <= 0 || (*bullet)->pos.y >= MAP_SIZE.y)
        //         {
        //             bullet = m_bullets.erase(bullet);
        //         }
        //         else  ++bullet;
        //     }
        // }
    };

    void SelectTarget(std::vector<std::shared_ptr<Enemy>> enemiesInRange)
    {
        // for (const auto& enemy : enemiesInRange)
        // {
        //     /* code to select target */
        // }

        // if (!enemiesInRange.empty())
        // {
        //     target.position = enemiesInRange[0]->data.GetRealPos();
        //     target.isExist = true;
        // }
        // else
        // {
        //     target.position = {0, 0};
        //     target.isExist = false;
        // }
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

                // std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(this);
                // bullet->position.grid = data.position.grid;
                // bullet->position.local = data.center;
                // bullet->prevLocalPosition = bullet->position.local;
                // bullet->speed = data.projectileSpeed;
                // bullet->damage = data.bulletDamage;
                // bullet->radius = 1.0f / TILE_SIZE;

                // // [todo] need to rethink how to handle bullet movement/position
                // Vector2 direction = target.position - bullet->position.local;
                // bullet->velocity = Vector2Normalize(direction);
                // bullet->velocity = {bullet->velocity.x / 10.0f, bullet->velocity.y / 10.0f};

                // m_bullets.push_back(bullet);
            }
        }
    };
    void DrawBullets();
    std::vector<std::shared_ptr<Bullet>> GetBullets() { return m_bullets; };

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

    void RemoveBullet(std::shared_ptr<Bullet> bullet)
    {
        // auto it = std::find_if(m_bullets.begin(), m_bullets.end(), [bullet](const std::shared_ptr<Bullet>& ptr) {
        //     return ptr == bullet;
        // });

        // if (it != m_bullets.end())
        // {
        //     m_bullets.erase(it);
        // }
    };

private:

    std::vector<std::shared_ptr<Bullet>> m_bullets;

    struct Target
    {
        bool isExist {false};
        Vec2f position {0.0, 0.0};
    };
    Target target;

    // Target target {Target::FIRST};
};

#endif