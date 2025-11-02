//
// Created by phatt on 11/1/25.
//

#include "Observer.h"

#include "Observable.h"

namespace ZPG {

Observer::~Observer() {
    for (Observable* observable : m_Observables) {
        observable->DetachObserver(this); // WARN: logic is done on the observable side
    }
}

void Observer::Observe(Observable* observable) {
    m_Observables.push_back(observable);
    observable->AttachObserver(this); // WARN: logic is done on the observable side
}

void Observer::StopObserving(Observable* observable) {
    auto it = std::ranges::find(m_Observables, observable);

    if (it != m_Observables.end()) {
        m_Observables.erase(it);
        (*it)->DetachObserver(this); // WARN: logic is done on the observable side
    }
}

}
