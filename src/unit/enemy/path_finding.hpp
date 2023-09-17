#ifndef UNIT_PATH_FINDING_HPP
#define UNIT_PATH_FINDING_HPP

#include "global_data.hpp"

#include "AStar.hpp"

class PathFinding
{
public:
    PathFinding() = default;
    ~PathFinding() = default;

    AStar::Generator generator;

    void SetGenerator(LevelData &map);
    void SetCollision(LevelData &map);

    AStar::CoordinateList GetPath(const AStar::Vec2i startPoint, const AStar::Vec2i endPoint);
};

#endif