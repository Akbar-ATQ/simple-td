#include "tower.hpp"

#include "grid_helper.hpp"
#include "ui.hpp"

#include "raymath.h"

void Tower::DrawDetectionRange()
{
    // [not implemented yet]
};

void Tower::Draw()
{
    GH::DrawRec(GetRec(), color);
};

void Tower::SelectTarget(std::vector<std::shared_ptr<Enemy>> enemiesInRange)
{
    // [not implemented yet]
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

void Tower::Shoot(std::vector<std::shared_ptr<Enemy>> enemiesInRange)
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

std::vector<std::shared_ptr<Bullet>> Tower::GetBullets()
{
    return bullets;
};

void Tower::Upgrade()
{
    // [not implemented yet]
};

void Tower::DrawStat(std::string text, float stat, int &order)
{
    DrawText(TextFormat("%s: %.2f", text.c_str(), stat),
        GH::RealPosition(UI::sidePanelPos.x),
        GH::RealPosition(UI::sidePanelPos.y + 2 + (static_cast<float>(order) * 0.5)),
        20,
        BLACK
    );

    order += 1;
}
void Tower::StatusPanel()
{
    UI::SidePanel();

    int order {0};
    DrawStat("Damage", bulletDamage, order);
    DrawStat("Range", detectionRange, order);
    DrawStat("Attack Speed", attackSpeed, order);
    DrawStat("Projectile Speed", projectileSpeed, order);
};

void Tower::MoveBullets(std::vector<std::shared_ptr<Bullet>> &otherBullets)
{
    otherBullets.insert(otherBullets.end(), std::make_move_iterator(bullets.begin()), std::make_move_iterator(bullets.end()));
    bullets.clear();
};
