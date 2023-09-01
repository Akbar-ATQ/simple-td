#ifndef UNIT_BULLET_HPP
#define UNIT_BULLET_HPP

class Bullet : public Entity
{
public:
    Bullet();
    ~Bullet();

    // std::shared_ptr<Event::Signal> hittingEnemy = 

    void Draw()
    {
        GH::DrawCirc(GH::MergeReal(position.grid, position.local), radius, BLACK);
    };

    void Update()
    {
        Move();
    };

    void Move()
    {
        if (position.grid.x != target.x)
        {
            if (position.grid.x > target.x)
                position.local.x -= speed * GetFrameTime();
            else
                position.local.x += speed * GetFrameTime();
        }
        else if (position.grid.y != target.y)
        {
            if (position.grid.y > target.y)
                position.local.y -= speed * GetFrameTime();
            else
                position.local.y += speed * GetFrameTime();
        }

        if (position.local.x > GRID_SIZE || position.local.y > GRID_SIZE ||
            position.local.x < 0 || position.local.y < 0
            )
        {
            MoveGrid(this, {target.x, target.y});
        }

        if (position.grid.x == target.x && position.grid.y == target.y)
        {
            m_path.index++;
        }
    };

private:
    Vec2i prevposition.grid;
    Vec2f prevLocalPosition;

    float radius;
    float speed;
    Vec2f velocity {0, 0};
    Vec2f target;

    float damage;
};

struct Bullet
    {
        Bullet(Tower* tower) : tower{tower} {};

        Vec2i position.grid;
        Vec2f position.local;
        Vec2i prevposition.grid;
        Vec2f prevLocalPosition;

        float radius;
        float speed;
        Vec2f velocity {0, 0};
        Vec2f target;

        float damage;

        void Update()
        {
            prevposition.grid = position.grid;
            prevLocalPosition = position.local;
            position.local += velocity * speed;
            // localPos.y += velocity.y * speed;
        };

        void RemoveFromTower(std::shared_ptr<Bullet> bullet) { tower->RemoveBullet(bullet); };
    private:
        Tower* tower;
    };

#endif