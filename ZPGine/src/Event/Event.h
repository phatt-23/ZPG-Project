//
// Created by phatt on 9/22/25.
//

#ifndef EVENT_H
#define EVENT_H

#include "Core/Core.h"
#include <functional>
#include <string>

namespace ZPG {

enum EventCategory : unsigned int {
    EventCategoryNone = ZPG_BIT(0),
    EventCategoryApplication = ZPG_BIT(1),
    EventCategoryWindow = ZPG_BIT(2),
    EventCategoryInput = ZPG_BIT(3),
    EventCategoryKeyBoard = ZPG_BIT(4),
    EventCategoryMouse = ZPG_BIT(5),
    EventCategoryMouseButton = ZPG_BIT(6),
};

enum class EventType : unsigned int {
    None, 
    AppTick, AppUpdate, AppRender,
    WindowClose, WindowResize, WindowFocus, WindowLostFocus, WindowMoved,
    KeyPressed, KeyReleased, KeyTyped,
    MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled,
};

class Event {
    friend class EventDispatcher;
public:
    virtual unsigned int GetEventCategory() const = 0;
    virtual EventType GetEventType() const = 0;
    virtual const char* GetName() const = 0;
    inline bool IsInCategory(EventCategory c) { return (unsigned int)GetEventCategory() & (unsigned int)c; }
    inline std::string ToString() const { return GetName(); }
    virtual bool IsHandled() const { return m_Handled; }
protected:
    bool m_Handled = false;
};

class EventDispatcher {
public: 
    EventDispatcher(Event& e) 
        : m_Event(e) 
    {}

    template<typename T>
    requires std::derived_from<T, Event> && requires { { T::GetStaticType() } -> std::convertible_to<EventType>; }
    using EventHandlerFn = std::function<bool(T&)>;

    template<typename T>
    bool Dispatch(EventHandlerFn<T>&& handler) {
        if (T::GetStaticType() == m_Event.GetEventType()) {
            m_Event.m_Handled = handler(*(T*)&m_Event);
            return true;
        }
        return false;
    }

private:
    Event& m_Event;
};

#define EVENT_CLASS_TYPE(type) \
    virtual EventType GetEventType() const override { return (type); } \
    static EventType GetStaticType() { return type; } \
    virtual const char* GetName() const override { return (#type); }

#define EVENT_CLASS_CATEGORY(category) \
    virtual unsigned int GetEventCategory() const override { return (category); }

}

#endif //EVENT_H
