#pragma once

namespace ZPG {

    class Observable;
    class Payload;

    class Observer {
    public:
        Observer() = default;
        virtual ~Observer();

        /**
         * Observable has something to say.
         */
        virtual void OnNotify(Payload& payload) = 0;

        /**
         * Listen to the observable.
         */
        void Observe(Observable* observable);

        /**
         * No longer want to listen to what the observable has to say.
         */
        void StopObserving(Observable* observable);


        // Don't make observers copyable, could copy internal links to observables they are listening to.
        // Upon observer's deconstructing, it detaches from all observables.
        Observer(const Observer&) = delete;
        Observer& operator=(const Observer&) = delete;

    private:
        friend class Observable;

        std::vector<Observable*> m_Observables;
    };

}
