#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "global_data.hpp"
#include "enemy.hpp"
#include "path_finding.hpp"
#include "grid.hpp"

#include "event_signal.hpp"
#include "event_list.hpp"

#include <vector>
#include <memory>

class Level
{
public:
    Level()
    {
        listener = Event::Signal::Create(this);
    };
    ~Level() = default;

    PathFinding pathFinding;
    Vec2i basePosition;

    std::shared_ptr<Event::Signal> listener;

    std::vector<std::shared_ptr<Enemy>> enemies;

    std::vector<std::vector<std::unique_ptr<Grid>>> map;

    void GenerateLevel(LevelData &levelData);

    void OnPlatformActivated(const Event::PlatformActivated platform);

    void Update();

    void Draw();
};

#endif