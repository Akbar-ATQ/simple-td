#ifndef GLOBALDATA_HPP
#define GLOBALDATA_HPP

#include "raylib.h"

#include <vector>
#include <variant>
#include <memory>

enum class Item
{
    ROAD,
    PLATFORM,
    BASE,
    SPAWN_POINT,
    EMPTY,
};

enum class TerrainID
{
    BASE,
    SPAWNER,
    ROAD,
    PLATFORM,
    EMPTY,
};

using LevelData = std::vector<std::vector<TerrainID>>;

class Road;
class SpawnPoint;
class Base;
class Platform;
struct Empty {};

struct MapData
{
    float x;
    float y;
    Item item;
    std::variant<
        std::shared_ptr<Base>,
        std::shared_ptr<Road>,
        std::shared_ptr<SpawnPoint>,
        std::shared_ptr<Platform>,
        std::shared_ptr<Empty>
    > obj {std::make_shared<Empty>()};
};

typedef std::vector<std::vector<MapData>> GridMap;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
// [todo] move tile variable to tile.hpp
const int TILE_SIZE = 40;
const Vector2 MAP_SIZE = {SCREEN_WIDTH / TILE_SIZE, SCREEN_HEIGHT / TILE_SIZE};

#endif