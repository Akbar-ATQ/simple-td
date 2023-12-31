#include "path_finding.hpp"

#include "grid_helper.hpp"

#include <vector>
#include <algorithm>

void PathFinding::SetGenerator(LevelData &map)
{
    generator.setWorldSize({static_cast<int>(MAP_SIZE.x), static_cast<int>(MAP_SIZE.y)});
    generator.setHeuristic(AStar::Heuristic::euclidean);
    generator.setDiagonalMovement(false);
    SetCollision(map);
};

void PathFinding::SetCollision(LevelData &map)
{
    std::vector<AStar::Vec2i> walls;
    auto AddWalls = [&walls, &map](const int x, const int y) 
    { 
        AStar::Vec2i below = {x, y + 1};
        if (!GH::OutsideMap(below.x, below.y) &&
            (map[below.x][below.y] == TerrainID::EMPTY || map[below.x][below.y] == TerrainID::PLATFORM)
        )
        {
            walls.push_back(below);
        }

        AStar::Vec2i right = {x + 1, y};
        if (!GH::OutsideMap(right.x, right.y) &&
            (map[right.x][right.y] == TerrainID::EMPTY || map[right.x][right.y] == TerrainID::PLATFORM)
        )
        {
            walls.push_back(right);
        }

        AStar::Vec2i above = {x, y - 1};
        if (!GH::OutsideMap(above.x, above.y) &&
            (map[above.x][above.y] == TerrainID::EMPTY || map[above.x][above.y] == TerrainID::PLATFORM)
        )
        {
            walls.push_back(above);
        }

        AStar::Vec2i left = {x - 1, y};
        if (!GH::OutsideMap(left.x, left.y) &&
            (map[left.x][left.y] == TerrainID::EMPTY || map[left.x][left.y] == TerrainID::PLATFORM)
        )
        {
            walls.push_back(left);
        }
    };

    for (int x = 0; x < map.size(); ++x)
    {
        for (int y = 0; y < map[x].size(); ++y)
        {
            if (map[x][y] == TerrainID::ROAD)
                AddWalls(x, y);
            if (map[x][y] == TerrainID::SPAWNER)
                AddWalls(x, y);
            if (map[x][y] == TerrainID::BASE)
                AddWalls(x, y);
        }
    }

    for (const auto& wall : walls)
    {
        generator.addCollision(wall);
    }
};

AStar::CoordinateList PathFinding::GetPath(const AStar::Vec2i startPoint, const AStar::Vec2i endPoint)
{
    auto path = generator.findPath(startPoint, endPoint);
    std::reverse(path.begin(), path.end());

    return path;
};
