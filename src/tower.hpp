#ifndef TOWER_HPP
#define TOWER_HPP

#include "entity_data.hpp"
#include "global_data.hpp"
#include "ui.hpp"
#include "enemy.hpp"

#include "raylib.h"
#include "raymath.h"

#include <vector>
#include <memory>

#include <iostream>

class Tower
{
public:
    Tower(Vector2 pos) : data{pos, GREEN} {};
    ~Tower() = default;

    struct Data : public EntityData
    {
        const Vector2 center;

        // ---------- Stats ---------- //
        float detectionRange {4};
        float attackTimer {0.0f};
        float attackSpeed {0.2f};
        float projectileSpeed {1.0f}; // 10.0 is the cap
        float bulletDamage {1};

        Data(Vector2 pos, Color color) : EntityData{pos, TILE_SIZE, color}, center{pos.x + 0.5f, pos.y + 0.5f} {};
    };

    struct Bullet
    {
        Bullet(Tower* tower) : tower{tower} {};

        Vector2 pos;
        Vector2 prevPos;
        float radius;
        float speed;
        Vector2 velocity {0, 0};
        Vector2 target;
        float damage;

        void Update()
        {
            prevPos = pos;
            pos.x += velocity.x * speed;
            pos.y += velocity.y * speed;
        };

        void RemoveFromTower(std::shared_ptr<Bullet> bullet) { tower->RemoveBullet(bullet); };
    private:
        Tower* tower;
    };

    Data data;

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
        Tile::DrawRec(data.GetRec(), data.color);

        for (const auto& bullet : m_bullets)
        {
            Tile::DrawCirc(bullet->pos, bullet->radius, BLACK);
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

        if (!enemiesInRange.empty())
        {
            target.position = enemiesInRange[0]->data.position;
            target.isExist = true;
        }
        else
        {
            target.position = {0, 0};
            target.isExist = false;
        }
    };
    void Shoot(std::vector<std::shared_ptr<Enemy>> enemiesInRange)
    {
        SelectTarget(enemiesInRange);

        if (target.isExist)
        {
            data.attackTimer += GetFrameTime();
            if (data.attackTimer >= data.attackSpeed)
            {
                data.attackTimer = 0.0f;

                std::shared_ptr<Bullet> bullet = std::make_shared<Bullet>(this);
                bullet->pos = data.center;
                bullet->prevPos = bullet->pos;
                bullet->speed = data.projectileSpeed;
                bullet->damage = data.bulletDamage;
                bullet->radius = 1.0f / TILE_SIZE;

                Vector2 direction = Vector2Subtract(target.position, bullet->pos);
                bullet->velocity = Vector2Normalize(direction);
                bullet->velocity = {bullet->velocity.x / 10.0f, bullet->velocity.y / 10.0f};

                m_bullets.push_back(bullet);
            }
        }
    };
    void DrawBullets();
    std::vector<std::shared_ptr<Bullet>> GetBullets() { return m_bullets; };

    void Upgrade();

    void DrawStat(std::string text, float stat)
    {
        DrawText(TextFormat("%s: %.3f", text.c_str(), stat),
            Tile::RealPosition(UI::sidePanel.x),
            Tile::RealPosition(UI::sidePanel.y + 2),
            14,
            BLACK
        );
    }
    void StatusPanel()
    {
        UI::SidePanel();
        DrawStat("Damage", data.bulletDamage);
    };

    void RemoveBullet(std::shared_ptr<Bullet> bullet)
    {
        auto it = std::find_if(m_bullets.begin(), m_bullets.end(), [bullet](const std::shared_ptr<Bullet>& ptr) {
            return ptr == bullet;
        });

        if (it != m_bullets.end())
        {
            m_bullets.erase(it);
        }
    };

private:

    std::vector<std::shared_ptr<Bullet>> m_bullets;

    struct Target
    {
        bool isExist {false};
        Vector2 position;
    };
    Target target;

    // Target target {Target::FIRST};
};

#endif