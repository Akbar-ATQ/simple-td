#ifndef LEVEL_LOADER_HPP
#define LEVEL_LOADER_HPP

#include "global_data.hpp"

#include <string>

namespace Level
{
    LevelData Loader(const std::string &levelPath);
};

#endif