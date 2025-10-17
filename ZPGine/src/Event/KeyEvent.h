//
// Created by phatt on 9/22/25.
//

#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include "Event.h"

namespace ZPG {

class KeyEvent : public Event {
public:
    KeyEvent(int keyCode) : m_KeyCode(keyCode) {
    }

    EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyBoard)
private:
    int m_KeyCode;
};

class KeyPressedEvent : public KeyEvent {
public:
    KeyPressedEvent(int keyCode, int repeatCount) 
        : KeyEvent(keyCode), m_RepeatCount(repeatCount)
    {}

    EVENT_CLASS_TYPE(EventType::KeyPressed)
private:
    int m_RepeatCount;
}; 

class KeyReleasedEvent : public KeyEvent {
public:
    KeyReleasedEvent(int keyCode) : KeyEvent(keyCode)
    {}

    EVENT_CLASS_TYPE(EventType::KeyReleased)
};

class KeyTypedEvent : public KeyEvent {
public:
    KeyTypedEvent(int keyCode) : KeyEvent(keyCode) 
    {}

    EVENT_CLASS_TYPE(EventType::KeyTyped)
};

}

#endif
