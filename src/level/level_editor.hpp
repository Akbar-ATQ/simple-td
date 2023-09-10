#ifndef LEVEL_EDITOR_HPP
#define LEVEL_EDITOR_HPP

#include "global_data.hpp"
#include "road.hpp"
#include "base.hpp"
#include "spawner.hpp"
#include "platform.hpp"

#include "raylib.h"

namespace Level
{
    class Editor
    {
    public:
        Editor()
        {
            for (int x = 0; x < MAP_SIZE.x; ++x)
            {
                std::vector<TerrainID> row;

                for (int y = 0; y < MAP_SIZE.y; ++y)
                {
                    row.push_back(TerrainID::EMPTY);
                }

                levelData.push_back(row);
            }

            Vec2i dummyPosition {0, 0};

            Road road {dummyPosition};
            dummyRoad = {road.size, road.color};

            Base base {dummyPosition};
            dummyBase = {base.size, base.color};

            Spawner spawner {dummyPosition};
            dummySpawner = {spawner.size, spawner.color};

            Platform platform {dummyPosition};
            dummyPlatform = {platform.size, platform.color};
        };
        ~Editor() = default;

        void EditLevel();
        void SaveLevel();

        void Update();

    private:
        LevelData levelData;
        TerrainID terrain {TerrainID::EMPTY};
        Vec2i selectedGrid {0, 0};

        struct DummyTerrain
        {
            float size;
            Color color;
        };

        DummyTerrain dummyRoad;
        DummyTerrain dummyBase;
        DummyTerrain dummySpawner;
        DummyTerrain dummyPlatform;

        void DrawDummyTerrain(const int x, const int y);
    };
};

#endif