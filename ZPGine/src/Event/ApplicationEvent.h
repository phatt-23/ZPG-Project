//
// Created by phatt on 9/22/25.
//

#ifndef APPLICATION_EVENT_H
#define APPLICATION_EVENT_H

#include "Event.h"

namespace ZPG {

class AppTick : public Event {
public:
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(EventType::AppTick)
};

class AppUpdate : public Event {
public:
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(EventType::AppUpdate)
};

class AppRender : public Event {
public:
    EVENT_CLASS_CATEGORY(EventCategoryApplication)
    EVENT_CLASS_TYPE(EventType::AppRender)
};

}

#endif
