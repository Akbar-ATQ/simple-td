#ifndef LEVEL_MANAGER_HPP
#define LEVEL_MANAGER_HPP

#include "global_data.hpp"
#include "enemy.hpp"
#include "path_finding.hpp"
#include "grid.hpp"

#include "event_signal.hpp"
#include "event_list.hpp"

#include <vector>
#include <memory>

namespace Level
{
    class Manager
    {
    public:
        Manager()
        {
            listener = Event::Signal::Create(this);
            listener->Connect<Event::PlatformActivated, Level::Manager>(Event::PlatformActivated(), &Level::Manager::OnPlatformActivated);
        };
        ~Manager() = default;

        PathFinding pathFinding;

        std::shared_ptr<Event::Signal> listener;

        std::vector<std::shared_ptr<Enemy>> enemies;

        std::vector<std::vector<std::unique_ptr<Grid>>> map;

        void GenerateLevel(LevelData &levelData);

        void OnPlatformActivated(const Event::PlatformActivated platform);

        void Clear();

        void Update();

        void Draw();
    };
};

#endif