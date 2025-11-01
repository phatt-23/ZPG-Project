//
// Created by phatt on 10/18/25.
//
#include "Observable.h"

#include "Observer.h"
#include "Event/Event.h"

namespace ZPG {

void Observable::AttachObserver(Observer* observer) {
    m_Observers.push_back(observer);
}
void Observable::DetachObserver(Observer* observer) {
    auto it = std::ranges::find(m_Observers, observer);
    if (it != m_Observers.end()) {
        m_Observers.erase(it);
    }
}
void Observable::NotifyWith(Payload& payload) const {
    for (Observer* observer : m_Observers) {
        observer->OnNotify(payload);
    }
}

}
