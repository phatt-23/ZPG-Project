//
// Created by phatt on 9/22/25.
//

#ifndef KEY_EVENT_H
#define KEY_EVENT_H

#include "Event.h"

namespace ZPG 
{
    class KeyEvent : public Event 
    {
    public:
        KeyEvent(int keyCode, int mods) 
            : m_KeyCode(keyCode) 
            , m_Mods(mods)
        {
        }

        int GetKeyCode() const { return m_KeyCode; }
        int GetMods() const { return m_Mods; }
        int IsMod(int mod) { return m_Mods & mod; }

        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyBoard)

    private:
        int m_KeyCode;
        int m_Mods;
    };

    class KeyPressedEvent : public KeyEvent 
    {
    public:
        KeyPressedEvent(int keyCode, int mods, int repeatCount) 
            : KeyEvent(keyCode, mods)
            , m_RepeatCount(repeatCount)
        {}

        EVENT_CLASS_TYPE(EventType::KeyPressed)
    private:
        int m_RepeatCount;
    }; 

    class KeyReleasedEvent : public KeyEvent 
    {
    public:
        KeyReleasedEvent(int keyCode, int mods) 
            : KeyEvent(keyCode, mods)
        {
        }

        EVENT_CLASS_TYPE(EventType::KeyReleased)
    };

    class KeyTypedEvent : public KeyEvent 
    {
    public:
        KeyTypedEvent(int keyCode) 
            : KeyEvent(keyCode, 0) 
        {
        }

        EVENT_CLASS_TYPE(EventType::KeyTyped)
    };

}

#endif
