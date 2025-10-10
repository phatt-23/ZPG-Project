#pragma once
#include <vector>
#include "Observer.h"

namespace ZPG {

class Observable {
public:
    virtual void AttachObserver(Observer* observer) { 
        m_Observers.push_back(observer); 
    }
    virtual void DetachObserver(Observer* observer) {
        auto it = std::find(m_Observers.begin(), m_Observers.end(), observer); 
        if (it != m_Observers.end()) {
            m_Observers.erase(it);
        }
    }
protected:
    void NotifyWith(Payload& payload) {
        for (Observer* observer : m_Observers) {
            observer->OnNotify(payload);
        }
    }
private:
    std::vector<Observer*> m_Observers;
};

}
