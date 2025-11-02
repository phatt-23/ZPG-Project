//
// Created by phatt on 10/18/25.
//
#include "Observable.h"

#include <utility>

#include "Observer.h"
#include "Event/Event.h"

namespace ZPG {

Observable::~Observable() {
    // telling my observers that i'm dead
    for (Observer* observer : m_Observers) {
        observer->StopObserving(this);
    }
}

void Observable::AttachObserver(Observer* observer) {
    m_Observers.push_back(observer);
    observer->m_Observables.push_back(this);
}

void Observable::DetachObserver(Observer* observer) {
    auto it = std::ranges::find(m_Observers, observer);
    if (it != m_Observers.end()) {
        m_Observers.erase(it);
        std::erase_if(observer->m_Observables, [this](Observable* obs){ return this == obs; });
    }
}

u32 Observable::AttachCallback(std::function<void(Payload& payload)> callback) {
    u32 id = m_NextCallbackID++;
    m_Callbacks[id] = std::move(callback);
    return id;
}

bool Observable::DetachCallback(u32 callbackID) {
    u32 ret =  m_Callbacks.erase(callbackID);
    return ret;
}

void Observable::NotifyWith(Payload& payload) const {
    // because observer could kill themselves while iterating through them
    std::vector<Observer*> observersCopy = m_Observers;
    std::unordered_map<u32, std::function<void(Payload& payload)>> callbacksCopy = m_Callbacks;

    for (Observer* observer : observersCopy) {
        observer->OnNotify(payload);
    }

    for (const std::function<void(Payload&)>& fn : callbacksCopy | std::views::values) {
        fn(payload);
    }
}

}
