#ifndef EVENT_SIGNAL_HPP
#define EVENT_SIGNAL_HPP

#include <vector>
#include <memory>
#include <functional>
#include <algorithm>
#include <any>
#include <string>
#include <type_traits>
#include <cassert>

namespace Event
{
    struct EventType
    {
        const std::string name;
        EventType(const std::string &eventName) : name(eventName) {};
        EventType() = delete; // Default constructor are deleted to make sure name are initialized

        bool operator==(const EventType &other) const { return (name == other.name); }
    };

    // struct ExampleEvent : public EventType
    // {
    //     ExampleEvent() : EventType{"ExampleEvent"} {};

    //     int exampleData;
    // };
};

// ---------- Event hash ---------- //
template <>
struct std::hash<Event::EventType>
{
    std::size_t operator()(const Event::EventType &key) const
    {
        using std::size_t;
        size_t hashKey = hash<std::string>{}(key.name);
        return hashKey;
    }
};

// ---------- Declaration ---------- //
namespace Event
{
    class Signal;

    class Manager
    {
    public:
        Manager() = default;
        ~Manager() = default;

        static std::shared_ptr<Manager> Create();

        void Connect(const std::weak_ptr<Signal> &signal);
        void Disconnect(const std::weak_ptr<Signal> &signal);

        void Connect(const std::weak_ptr<Manager> &manager);
        void Disconnect(const std::weak_ptr<Manager> &manager);

        template<typename E>
        void Emit(const E &event);
        template<typename E, typename ...I>
        void Emit(const E &event, const I &...items);

    private:
        std::vector<std::weak_ptr<Signal>> signals;
        std::vector<std::weak_ptr<Manager>> managers;
    };

    class Signal : public std::enable_shared_from_this<Signal>
    {
    public:
        Signal() = default;
        Signal(const void *obj) : objComponent{obj} {};
        ~Signal() = default;

        static std::shared_ptr<Signal> Create();
        static std::shared_ptr<Signal> Create(void *obj);

        void Connect(const std::shared_ptr<Manager> &manager);
        void Disconnect(const std::shared_ptr<Manager> &manager);

        // ---------- Handle slot ---------- //
        template<typename E>
        using Slot = std::function<void(const E&)>;
        template<typename E, typename O>
        using MethodSlot = std::function<void(O&, const E&)>;

        template<typename E>
        void Connect(const E &event, const Slot<E> slot);

        template<typename E, typename O>
        void Connect(const E &event, const MethodSlot<E, O> methodSlot, const std::weak_ptr<O> &obj);

        template<typename E, typename O>
        void Connect(const E &event, const MethodSlot<E, O> methodSlot, O *obj);

        template<typename E, typename O>
        void Connect(const E &event, const MethodSlot<E, O> methodSlot);

        template<typename E>
        void Disconnect(const E &event);
        // ------------------------------ //

        template<typename E>
        void Emit(const E &event);

    private:
        std::unordered_map<EventType, std::any> connectedSlot;
        const void *objComponent {nullptr};
    };

    class TrackSignalRaw
    {
    public:
        TrackSignalRaw() = default;
        ~TrackSignalRaw();

        template<typename E>
        void TrackConnection(const E &event, const std::weak_ptr<Signal> signal);

    private:
        std::vector<std::weak_ptr<Signal>> signals;
        std::vector<EventType> events;
    };
};

// ---------- Implementation ---------- //
namespace Event
{
    // ---------- Manager ---------- //
    inline std::shared_ptr<Manager> Manager::Create()
    {
        return std::make_shared<Manager>();
    };

    inline void Manager::Connect(const std::weak_ptr<Signal> &signal)
    {
        signals.push_back(signal);
    };
    inline void Manager::Disconnect(const std::weak_ptr<Signal> &signal)
    {
        signals.erase(std::remove_if(signals.begin(), signals.end(),
            [&signal](const std::weak_ptr<Signal> &element)
            {
                return element.lock() == signal.lock();
            }), signals.end());
    };

    inline void Manager::Connect(const std::weak_ptr<Manager> &manager)
    {
        managers.push_back(manager);
    };
    inline void Manager::Disconnect(const std::weak_ptr<Manager> &manager)
    {
        managers.erase(std::remove_if(managers.begin(), managers.end(),
            [&manager](const std::weak_ptr<Manager> &element)
            {
                return element.lock() == manager.lock();
            }), managers.end());
    };

    template<typename E>
    inline void Manager::Emit(const E &event)
    {
        auto handleEmit = [&event](auto &items)
        {
            bool expired {false};

            for (const auto &item : items)
            {
                const auto lockedItem = item.lock();
                if (lockedItem)
                {
                    lockedItem->Emit(event);
                }
                else expired = true;
            }

            if (expired)
            {
                items.erase(std::remove_if(items.begin(), items.end(),
                    [](const auto &item)
                    {
                        return item.expired();
                    }), items.end());
            }
        };

        if (!signals.empty()) handleEmit(signals);
        if (!managers.empty()) handleEmit(managers);
    };

    template<typename E, typename ...I>
    inline void Manager::Emit(const E &event, const I &...items)
    {
        std::vector<std::weak_ptr<Manager>> emitManagers;
        std::vector<std::weak_ptr<Signal>> emitSignals;

        auto handleParams = [&emitManagers, &emitSignals](const auto &item)
        {
            using ItemType = std::decay_t<decltype(item)>;
            static_assert(
                (
                    std::is_same_v<ItemType, std::shared_ptr<Manager>> ||
                    std::is_same_v<ItemType, std::shared_ptr<Signal>>
                ),
                "Can only emit to Event::Manager or Event::Signal."
            );

            if constexpr (std::is_same_v<ItemType, std::shared_ptr<Manager>>)
            {
                emitManagers.push_back(item);
            }
            else if constexpr (std::is_same_v<ItemType, std::shared_ptr<Signal>>)
            {
                emitSignals.push_back(item);
            }
        };

        (handleParams(items), ...);

        if (!emitSignals.empty())
        {
            for (const auto &signal : emitSignals)
            {
                auto lockedSignal = signal.lock();
                if (lockedSignal) lockedSignal->Emit(event);
            }
        }

        if (!emitManagers.empty())
        {
            for (const auto &manager : emitManagers)
            {
                auto lockedManager = manager.lock();
                if (lockedManager) lockedManager->Emit(event);
            }
        }
    };

    // ---------- Signal ---------- //
    inline std::shared_ptr<Signal> Signal::Create()
    {
        return std::make_shared<Signal>();
    };
    inline std::shared_ptr<Signal> Signal::Create(void *obj)
    {
        return std::make_shared<Signal>(obj);
    };

    inline void Signal::Connect(const std::shared_ptr<Manager> &manager)
    {
        manager->Connect(shared_from_this());;
    };
    inline void Signal::Disconnect(const std::shared_ptr<Manager> &manager)
    {
        manager->Disconnect(shared_from_this());
    };

    template<typename E>
    inline void Signal::Connect(const E &event, const Slot<E> slot)
    {
        // The main `Connect()`.
        // Set free function as slot.
        // Other `Connect()` turn `MethodSlot` into `Slot` and call this `Connect()`.

        static_assert(std::is_base_of<EventType, E>::value, "slot parameter must be event inheriting Event::EventType");

        connectedSlot[event] = slot;
    };

    template<typename E, typename O>
    inline void Signal::Connect(const E &event, const MethodSlot<E, O> methodSlot, const std::weak_ptr<O> &obj)
    {
        const Slot<E> slot = [this, obj, methodSlot](const E &eventData)
        {
            auto lockedObj = obj.lock();
            if (lockedObj) methodSlot(*lockedObj, eventData);
            else this->Disconnect(eventData);
        };

        Connect(event, slot);
    };

    template<typename E, typename O>
    inline void Signal::Connect(const E &event, const MethodSlot<E, O> methodSlot, O *obj)
    {
        static_assert(std::is_base_of<TrackSignalRaw, O>::value,
            "Your class must inherit Event::TrackSignalRaw if you want to use raw pointer"
        );

        obj->TrackConnection(event, shared_from_this());

        const Slot<E> slot = [this, obj, methodSlot](const E &eventData)
        {
            if (obj) methodSlot(*obj, eventData);
            else this->Disconnect(eventData);
        };

        Connect(event, slot);
    };

    template<typename E, typename O>
    inline void Signal::Connect(const E &event, const MethodSlot<E, O> methodSlot)
    {
        assert(objComponent != nullptr &&
            "If you use signal as class component, pass `this` to signal to bypass the need of inheriting Event::TrackSignalRaw"
        );

        const Slot<E> slot = [this, methodSlot](const E &eventData)
        {
            methodSlot(*const_cast<O*>(static_cast<const O*>(this->objComponent)), eventData);
        };

        Connect(event, slot);
    };

    template<typename E>
    inline void Signal::Disconnect(const E &event)
    {
        connectedSlot.erase(event);
    };

    template<typename E>
    inline void Signal::Emit(const E &event)
    {
        static_assert(std::is_base_of<EventType, E>::value, "Event must inherit Event::EventType");

        if (connectedSlot.count(event) > 0)
        {
            std::any_cast<Slot<E>>(connectedSlot[event])(event);
        }
    };

    // ---------- Tracking connection without shared_ptr ---------- //
    template<typename E>
    inline void TrackSignalRaw::TrackConnection(const E &event, const std::weak_ptr<Signal> signal)
    {
        signals.push_back(signal);
        events.push_back(event);
    };

    inline TrackSignalRaw::~TrackSignalRaw()
    {
        for (const auto &signal : signals)
        {
            auto lockedSignal = signal.lock();
            if (lockedSignal)
            {
                for (const auto &event : events)
                {
                    lockedSignal->Disconnect(event);
                }
            }
        }
    };
};

#endif