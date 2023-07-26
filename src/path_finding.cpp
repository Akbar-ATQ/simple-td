#include "path_finding.hpp"

#include <vector>

void PathFinding::SetGenerator(GridMap& map)
{
    generator.setWorldSize({static_cast<int>(MAP_SIZE.x), static_cast<int>(MAP_SIZE.y)});
    generator.setHeuristic(AStar::Heuristic::euclidean);
    generator.setDiagonalMovement(false);
    SetCollision(map);
};

void PathFinding::SetCollision(GridMap& map)
{
    std::vector<AStar::Vec2i> walls;
    auto AddWalls = [&walls, &map](const int x, const int y) 
    { 
        AStar::Vec2i below = {x, y + 1};
        if (map[below.x][below.y].item == Item::EMPTY)
        {
            walls.push_back(below);
        }

        AStar::Vec2i right = {x + 1, y};
        if (map[right.x][right.y].item == Item::EMPTY)
        {
            walls.push_back(right);
        }

        AStar::Vec2i above = {x, y - 1};
        if (map[above.x][above.y].item == Item::EMPTY)
        {
            walls.push_back(above);
        }

        AStar::Vec2i left = {x - 1, y};
        if (map[left.x][left.y].item == Item::EMPTY)
        {
            walls.push_back(left);
        }
    };

    for (int x = 0; x < map.size(); ++x)
    {
        for (int y = 0; y < map[x].size(); ++y)
        {
            if (map[x][y].item == Item::ROAD) AddWalls(x, y);
        }
    }

    for (const auto& wall : walls)
    {
        generator.addCollision(wall);
    }
};

AStar::CoordinateList PathFinding::GetPath(const AStar::Vec2i startPoint, const AStar::Vec2i endPoint)
{
    return generator.findPath(startPoint, endPoint);
};
