#ifndef PATHFINDING_HPP
#define PATHFINDING_HPP

#include "global_data.hpp"

#include "AStar.hpp"

class PathFinding
{
public:
    PathFinding() = default;
    ~PathFinding() = default;

    void SetGenerator(GridMap& map);
    void SetCollision(GridMap& map);

    AStar::CoordinateList GetPath(const AStar::Vec2i startPoint, const AStar::Vec2i endPoint);

private:
    AStar::Generator generator;
};

#endif