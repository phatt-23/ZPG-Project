//
// Created by phatt on 9/22/25.
//

#ifndef MOUSE_EVENT_H
#define MOUSE_EVENT_H

#include "Event.h"

namespace ZPG {

class MouseEvent : public Event {
};

class MouseMovedEvent : public MouseEvent {
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

class MouseScrolledEvent : public MouseEvent {
public:
    MouseScrolledEvent(float xOffset, float yOffset) : m_XOffset(xOffset), m_YOffset(yOffset) {}
    EVENT_CLASS_CATEGORY(EventCategoryMouse)
    EVENT_CLASS_TYPE(EventType::MouseScrolled)

    f32 GetXOffset() const { return m_XOffset; }
    f32 GetYOffset() const { return m_YOffset; }
    std::pair<f32,f32> GetOffset() const { return {m_XOffset, m_YOffset}; }

private:
    float m_XOffset;
    float m_YOffset;
};

class MouseButtonEvent : public MouseEvent {
public:
    MouseButtonEvent(int buttonCode) : m_ButtonCode(buttonCode) {}
    int GetButtonCode() const { return m_ButtonCode; }

    EVENT_CLASS_CATEGORY(EventCategoryMouse | EventCategoryMouseButton)
private:
    int m_ButtonCode;
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
    MouseButtonPressedEvent(int buttonCode)
        : MouseButtonEvent(buttonCode)
    {}

    EVENT_CLASS_TYPE(EventType::MouseButtonPressed)
private:
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    MouseButtonReleasedEvent(int buttonCode)
        : MouseButtonEvent(buttonCode)
    {}
    EVENT_CLASS_TYPE(EventType::MouseButtonReleased)
private:
};

}

#endif
