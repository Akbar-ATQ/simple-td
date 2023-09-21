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

    if (upgradeNum)
    {
        GH::DrawTriangleInCenter(position.grid, 10, RED);
    }
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
            listener->Connect(bullet->event);
            bullets.push_back(bullet);
        }
    }
};

void Tower::OnHitEnemy(Event::HitEnemy hitEnemy)
{
    exp += hitEnemy.exp;
};

void Tower::LevelUp()
{
    if (exp >= nextLevelExp)
    {
        level += 1;
        exp -= nextLevelExp;
        nextLevelExp *= 1.2;
        upgradeNum += 1;
    }
};

void Tower::Upgrade(Stat stat, float &value, int order, int textSize)
{
    float halfGrid = (GRID_SIZE / 2);
    Vector2 v1 {
        GH::RealPosition(UI::sidePanelPos.x + 3) - halfGrid,
        GH::RealPosition(UI::sidePanelPos.y + 2 + (static_cast<float>(order) * 0.5))
    };
    Vector2 v2 {
        GH::RealPosition(UI::sidePanelPos.x + 3) - GRID_SIZE,
        GH::RealPosition(UI::sidePanelPos.y + 2 + (static_cast<float>(order) * 0.5)) + (textSize / 2)
    };
    Vector2 v3 {
        GH::RealPosition(UI::sidePanelPos.x + 3),
        GH::RealPosition(UI::sidePanelPos.y + 2 + (static_cast<float>(order) * 0.5)) + (textSize / 2)
    };
    
    DrawTriangle(v1, v2, v3, RED);

    Vector2 textPos {
        GH::RealPosition(UI::sidePanelPos.x),
        GH::RealPosition(UI::sidePanelPos.y + 2 + (static_cast<float>(order) * 0.5))
    };
    Rectangle textRec {textPos.x, textPos.y, (GRID_SIZE * 3), static_cast<float>(textSize)};

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(GetMousePosition(), textRec))
    {
        if (stat == Stat::ATK_SPD)
            value *= 0.8;
        else
            value *= 1.2;

        upgradeNum -= 1;
    }
};

void Tower::DrawStat(std::string text, const float value, int &order, const int textSize)
{
    DrawText(TextFormat("%s: %.2f", text.c_str(), value),
        GH::RealPosition(UI::sidePanelPos.x),
        GH::RealPosition(UI::sidePanelPos.y + 2 + (static_cast<float>(order) * 0.5)),
        textSize,
        BLACK
    );

    order += 1;
}

void Tower::StatusPanel()
{
    UI::SidePanel();

    int order {0};
    int textSize {16};
    DrawStat("Level", level, order, textSize);
    DrawStat("EXP", exp, order, textSize);

    if (upgradeNum)
        Upgrade(Stat::DMG, bulletDamage, order, textSize);
    DrawStat("DMG", bulletDamage, order, textSize);

    if (upgradeNum)
        Upgrade(Stat::RANGE, detectionRange, order, textSize);
    DrawStat("Range", detectionRange, order, textSize);

    if (upgradeNum)
        Upgrade(Stat::ATK_SPD, attackSpeed, order, textSize);
    DrawStat("Atk.Spd", attackSpeed, order, textSize);

    if (upgradeNum)
        Upgrade(Stat::PROJECTILE_SPD, projectileSpeed, order, textSize);
    DrawStat("Proj.Spd", projectileSpeed, order, textSize);
};

std::vector<std::shared_ptr<Bullet>> Tower::GetBullets()
{
    return bullets;
};

void Tower::MoveBullets(std::vector<std::shared_ptr<Bullet>> &otherBullets)
{
    otherBullets.insert(otherBullets.end(), std::make_move_iterator(bullets.begin()), std::make_move_iterator(bullets.end()));
    bullets.clear();
};
