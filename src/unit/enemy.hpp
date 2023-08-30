#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "entity_data.hpp"
#include "tile.hpp"

#include "raylib.h"
#include "AStar.hpp"

// task:
// make tower shoot enemy

#include <iostream>

class Enemy
{
public:
    Enemy(Vector2 pos, AStar::CoordinateList& path) : data{pos, 20, RED, 5, 55.5}, m_path{path}
    {
        // [BUG]
        // when the path direction is changing, it don't perserve the original offset
        // the result is the enemies look like lining and don't look very good
        // for (auto& coordinate : path)
        // {
        //     m_path.coordinate.push_back({
        //         coordinate.x + (m_rec.x - std::floor(m_rec.x)),
        //         coordinate.y + (m_rec.y - std::floor(m_rec.y))
        //     });
        // }
    };
    ~Enemy() = default;

    struct Data : public MovingEntityData
    {
        Data(Vector2 pos, float size, Color color, int hp, float speed)
        : MovingEntityData{pos, size, color}, hp{hp}, speed{speed} {};

        int hp;
        float speed;
    };

    Data data;

    void TakeDamage(int damage) { data.hp -= damage; };
    // void AddPath(const AStar::CoordinateList& path) { m_path.coordinate = path; };
    void Move()
    {
        if (m_path.index < m_path.coordinate.size())
        {
            int targetX = m_path.coordinate[m_path.index].x;
            int targetY = m_path.coordinate[m_path.index].y;


            // if (static_cast<int>(data.position.x - (data.size / 100.0f)) != targetX)
            // {
            //     if (static_cast<int>(data.position.x - (data.size / 100.0f)) > targetX)
            //         data.position.x -= data.speed * GetFrameTime();
            //     else
            //         data.position.x += data.speed * GetFrameTime();
            // }
            // else if (static_cast<int>(data.position.y - (data.size / 100.0f)) != targetY)
            // {
            //     if (static_cast<int>(data.position.y - (data.size / 100.0f)) > targetY)
            //         data.position.y -= data.speed * GetFrameTime();
            //     else
            //         data.position.y += data.speed * GetFrameTime();
            // }
            // std::cout << "gridX: " << data.gridPosition.x << " targetX: " << targetX << " localX: " << data.localPosition.x << std::endl;
            // std::cout << "gridY: " << data.gridPosition.y << " targetY: " << targetY << " localY: " << data.localPosition.y << std::endl;
            if (data.gridPosition.x != targetX)
            {
                if (data.gridPosition.x > targetX)
                    data.localPosition.x -= data.speed * GetFrameTime();
                else
                    data.localPosition.x += data.speed * GetFrameTime();
            }
            else if (data.gridPosition.y != targetY)
            {
                if (data.gridPosition.y > targetY)
                    data.localPosition.y -= data.speed * GetFrameTime();
                else
                    data.localPosition.y += data.speed * GetFrameTime();
            }

            if (data.localPosition.x > TILE_SIZE || data.localPosition.y > TILE_SIZE ||
                data.localPosition.x < 0 || data.localPosition.y < 0
                )
            {
                // std::cout << "gridX: " << data.gridPosition.x << " targetX: " << targetX << " localX: " << data.localPosition.x << std::endl;
                // std::cout << "gridY: " << data.gridPosition.y << " targetY: " << targetY << " localY: " << data.localPosition.y << std::endl;
                data.MoveGrid({targetX, targetY});
            }

            if (static_cast<int>(data.gridPosition.x) == targetX && static_cast<int>(data.gridPosition.y) == targetY)
            {
                m_path.index++;
            }
        }
    };

    void Update()
    {
        Move();
    };
    void Draw()
    {
        Tile::DrawRec(data.GetRec(), data.color);
    };

private:
    struct Path
    {
        AStar::CoordinateList coordinate;
        int index {0};
    };
    Path m_path;
};

#endif