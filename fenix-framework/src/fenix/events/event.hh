#pragma once

#include <string>
#include <functional>

#include "fenix/core/base.hh"

namespace fenix {

    // Events in fenix are currently blocking, which means that when an event occurs
    // it immediately gets dispatched and must be dealt with right then and there.
    // For the future, a better strategy might be to buffer events in an event
    // bus and process them during the "event" part of the update stage.

    enum class EventType
    {
        None = 0,
        WindowClose,
        WindowResize,
        WindowFocus,
        WindowLostFocus,
        WindowMoved,
        KeyPressed,
        KeyReleased,
        KeyTyped,
        MouseButtonPressed,
        MouseButtonReleased,
        MouseMoved,
        MouseScrolled
    };

    enum EventCategory
    {
        None                     = 0,
        EventCategoryApplication = BIT(0),
        EventCategoryInput       = BIT(1),
        EventCategoryKeyboard    = BIT(2),
        EventCategoryMouse       = BIT(3),
        EventCategoryMouseButton = BIT(4)
    };

#define EVENT_CLASS_TYPE(type)                      \
    static EventType GetStaticType()                \
    {                                               \
        return EventType::type;                     \
    }                                               \
    virtual EventType GetEventType() const override \
    {                                               \
        return GetStaticType();                     \
    }                                               \
    virtual const char* GetName() const override    \
    {                                               \
        return #type;                               \
    }

#define EVENT_CLASS_CATEGORY(category)            \
    virtual int GetCategoryFlags() const override \
    {                                             \
        return category;                          \
    }

    /// Abstract event class from which specific events will inherit.
    class Event
    {
    public:
        virtual ~Event() = default;

        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }

    public:
        bool handled = false;
    };

    /// Decides which function to call when a certain event is received.
    class EventDispatcher
    {
    public:
        EventDispatcher(Event& event)
            : m_Event(event) {}

        // F will be deduced by the compiler
        template <typename T, typename F>
        bool Dispatch(const F& func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.handled |= func(static_cast<T&>(m_Event));
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };

    // NOTE(Miguel): Here, the `inline` keyword tells the compiler that there should only be one
    // definition of this function. C++ man...

    /// Overload the << operator so we can debug print Event types.
    inline std::ostream& operator<<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }

} // namespace fenix
