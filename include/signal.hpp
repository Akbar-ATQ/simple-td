#ifndef SIGNAL_HPP
#define SIGNAL_HPP

#include "raylib.h"

#include <vector>
#include <memory>
#include <functional>

namespace Signal
{
    enum class Event
    {
        REMOVE,
        ACTIVATING_TOWER,
    };

    struct EventData
    {
        Event event;
        Vector2 position;
    };

    class Listener
    {
    public:
        Listener() = default;
        ~Listener() = default;

        using SignalCallback = std::function<void(EventData)>;

        void ReceiveSignal(EventData eventData)
        {
            if (m_callback) m_callback(eventData);
        };

        void SetCallback(SignalCallback callback)
        {
            m_callback = callback;
        }

    private:
        SignalCallback m_callback;
    };

    class Signal
    {
    public:
        Signal() = default;
        ~Signal() = default;

        void Connect(std::shared_ptr<Listener> listener)
        {
            m_listeners.push_back(listener);
        };

        void Notify(EventData eventData)
        {
            for (const auto& listener : m_listeners)
            {
                listener->ReceiveSignal(eventData);
            }
        };

    private:
        std::vector<std::shared_ptr<Listener>> m_listeners;
    };
};

#endif