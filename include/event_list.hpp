#ifndef EVENT_LIST_HPP
#define EVENT_LIST_HPP

#include "event_signal.hpp"

#include "raylib.h"

namespace Event
{
    struct PlatformActivated : public EventType
    {
        PlatformActivated() : EventType{"PlatformActivated"} {};

        Vector2 position;
    };

    struct TowerAdded : public EventType
    {
        TowerAdded() : EventType{"TowerAdded"} {};

        Vector2 position;
    };
}

#endif