#include "level_loader.hpp"

#include <fstream>

LevelData Level::Loader(const std::string &levelPath)
{
    LevelData levelData;

    std::ifstream levelFile(levelPath, std::ios::binary);
    if (levelFile.is_open())
    {
        std::string line;
        while (std::getline(levelFile, line))
        {
            std::vector<TerrainID> row;
            for (const char c : line)
            {
                int id = c - '0';
                row.push_back(static_cast<TerrainID>(id));
            }
            levelData.push_back(row);
        }

        levelFile.close();
    }

    return levelData;
};