#ifndef EVENT_LIST_HPP
#define EVENT_LIST_HPP

#include "event_signal.hpp"
#include "global_data.hpp"

#include "raylib.h"

namespace Event
{
    struct PlatformActivated : public EventType
    {
        PlatformActivated() : EventType{"PlatformActivated"} {};

        Position position;
    };

    struct TowerAdded : public EventType
    {
        TowerAdded() : EventType{"TowerAdded"} {};

        Position position;
    };
}

#endif