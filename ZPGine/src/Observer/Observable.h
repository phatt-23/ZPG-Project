#pragma once


namespace ZPG {

class Observer;
class Payload;

class Observable {
public:
    virtual ~Observable();

    /**
     * Object-Oriented API.
     */

    /**
     * Add an observer to tell stuff to.
     */
    virtual void AttachObserver(Observer* observer);

    /**
     *  No longer want to tell observer anything.
     */
    virtual void DetachObserver(Observer* observer);


    /**
     * Functional API.
     */

    /**
     * Calls the callback whenever the observable notifies.
     * @param callback lambda callback
     * @return internal callbackID
     */
    virtual u32 AttachCallback(std::function<void(Payload& payload)> callback);

    /**
     * Tries to remove the callback from the observable.
     * @param callbackID the id of the callback lambda
     * @return 'true' on success, 'false' otherwise
     */
    virtual bool DetachCallback(u32 callbackID);

protected:

    /**
     * Tell the observers something.
     */
    void NotifyWith(Payload& payload) const;

private:
    std::vector<Observer*> m_Observers;

    std::unordered_map<u32, std::function<void(Payload& payload)>> m_Callbacks;

    mutable u32 m_NextCallbackID = 0;
};

}
