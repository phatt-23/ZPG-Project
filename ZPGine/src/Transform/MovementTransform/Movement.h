#pragma once

#include "Transform/Transform.h"

namespace ZPG
{
    class Movement : public Transform
    {
    public:
        Movement(float duration, float currentTime = 0);

        virtual ~Movement();

        virtual void Update(Timestep &ts) override;
        
        float GetCurrentTime() const { return m_CurrentTime; }
        float GetDuration() const { return m_Duration; }
        void SetDuration(float duration) { m_Duration = duration; }

    protected:
        float m_CurrentTime; // in seconds
        float m_Duration;    // in seconds
    };
}
