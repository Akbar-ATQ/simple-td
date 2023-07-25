#ifndef LEVELMANAGER_HPP
#define LEVELMANAGER_HPP

#include "platform.hpp"
#include "entity_data.hpp"
#include "global_data.hpp"
#include "tile.hpp"

#include "signal.hpp"

#include "raylib.h"

#include <vector>
#include <functional>
#include <memory>

class Road
{
public:
    Road(Vector2 pos) : data{pos, TILE_SIZE, BLUE} {};

    EntityData data;

    void Draw() { Tile::DrawRec(data.GetRec(), data.color); };
};

class Base
{
public:
    Base(Vector2 pos) : data{pos, TILE_SIZE, RED} {};

    EntityData data;

    void Draw() { Tile::DrawRec(data.GetRec(), data.color); };
};

class SpawnPoint
{
public:
    SpawnPoint(Vector2 pos) : data{pos, TILE_SIZE, PURPLE} {};

    EntityData data;

    void Draw() { Tile::DrawRec(data.GetRec(), data.color); };
};

// ------------------------------ //

class LevelManager
{
public:
    LevelManager()
    {
        listener->SetCallback(std::bind(&LevelManager::HandleSignalEvent, this, std::placeholders::_1));
    };
    ~LevelManager() = default;

    void GenerateLevel(GridMap& levelMap);

    void HandleSignalEvent(Signal::EventData eventData);

    void Update();
    void Draw();

private:
    struct Level
    {
        GridMap map;

        template<typename T>
        std::shared_ptr<T> GetObj(int x, int y) { return std::get<std::shared_ptr<T>>(map[x][y].obj); };
        template<typename T>
        std::shared_ptr<T> GetObj(Vector2 v ){ return std::get<std::shared_ptr<T>>(map[v.x][v.y].obj); };
    };
    Level level;

    std::shared_ptr<Signal::Listener> listener = std::make_shared<Signal::Listener>();
};

#endif