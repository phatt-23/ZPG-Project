#ifndef INCLUDE_EVENT_WINDOWEVENT_H_
#define INCLUDE_EVENT_WINDOWEVENT_H_

#include "Event.h"

namespace ZPG {

class WindowEvent : public Event {
public:
    EVENT_CLASS_CATEGORY(EventCategoryWindow)
};

class WindowCloseEvent : public WindowEvent {
public:
    EVENT_CLASS_TYPE(EventType::WindowClose)
};

class WindowResizeEvent : public WindowEvent {
public:
    EVENT_CLASS_TYPE(EventType::WindowResize)
    WindowResizeEvent(int width, int height)
        : m_Width(width), m_Height(height) 
    {}
    
    int GetWidth() const { return m_Width; }
    int GetHeight() const { return m_Height; }

private:
    int m_Width, m_Height;
};

class WindowFocusEvent : public WindowEvent {
public:
    WindowFocusEvent() = default;
    EVENT_CLASS_TYPE(EventType::WindowFocus)
};

class WindowLostFocusEvent : public WindowEvent {
public:
    WindowLostFocusEvent() = default;
    EVENT_CLASS_TYPE(EventType::WindowLostFocus)
};

class WindowMovedEvent : public WindowEvent {
public:
    WindowMovedEvent(int xpos, int ypos)
        : m_XPos(xpos), m_YPos(ypos)
    {}
    EVENT_CLASS_TYPE(EventType::WindowMoved)
private:
    int m_XPos, m_YPos;
};

}
#endif  // INCLUDE_EVENT_WINDOWEVENT_H_
