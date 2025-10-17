#pragma once


namespace ZPG {

class Observer;
class Payload;

class Observable {
public:
    virtual ~Observable() = default;
    virtual void AttachObserver(Observer* observer);
    virtual void DetachObserver(Observer* observer);
protected:
    void NotifyWith(Payload& payload);
private:
    std::vector<Observer*> m_Observers;
};

}
