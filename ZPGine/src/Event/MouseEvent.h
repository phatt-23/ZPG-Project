//
// Created by phatt on 9/22/25.
//

#ifndef MOUSEEVENT_H
#define MOUSEEVENT_H

#include "Event.h"

namespace ZPG {

class MouseMovedEvent : public Event {
public:
    MouseMovedEvent(float xPos, float yPos) : m_XPos(xPos), m_YPos(yPos) {}
    EVENT_CLASS_CATEGORY(EventCategoryMouse)
    EVENT_CLASS_TYPE(EventType::MouseMoved)
    
    f32 GetXPos() const { return m_XPos; }
    f32 GetYPos() const { return m_YPos; }
    std::pair<f32,f32> GetPos() const { return {m_XPos, m_YPos}; }
private:
    float m_XPos;
    float m_YPos;
};

class MouseScrolledEvent : public Event {
public:
    MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}
    EVENT_CLASS_CATEGORY(EventCategoryMouse)
    EVENT_CLASS_TYPE(EventType::MouseScrolled)
private:
    float m_XOffset;
    float m_YOffset;
};

class MouseButton : public Event {
public:
    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton)
};

class MouseButtonPressedEvent : public MouseButton {
public:
    MouseButtonPressedEvent(int buttonCode)
        : m_ButtonCode(buttonCode)
    {}

    EVENT_CLASS_TYPE(EventType::MouseButtonPressed)
private:
    int m_ButtonCode;
};

class MouseButtonReleasedEvent : public MouseButton {
public:
    MouseButtonReleasedEvent(int buttonCode)
        : m_ButtonCode(buttonCode)
    {}
    EVENT_CLASS_TYPE(EventType::MouseButtonReleased)
private:    
    int m_ButtonCode;
};

}

#endif //MOUSEEVENT_H
