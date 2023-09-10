#ifndef LEVEL_LOADER_HPP
#define LEVEL_LOADER_HPP

#include "global_data.hpp"

#include <string>

namespace Level
{
    void Loader(const std::string &levelPath, LevelData &levelData);
};

#endif