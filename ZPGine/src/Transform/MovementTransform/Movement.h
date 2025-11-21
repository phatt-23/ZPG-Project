#pragma once

#include "Transform/Transform.h"

namespace ZPG
{
    enum class MovementMode : u32
    {
        None = 0,
        PingPong,
        Repeat,
        Once,
        Uncapped,
    };

    class Movement : public Transform
    {
    public:
        Movement(float duration, MovementMode mode = MovementMode::Once, float currentTime = 0);

        virtual ~Movement();

        virtual void Update(Timestep &ts) override;
        
        virtual v3 GetCurrentPosition() = 0;

        float GetCurrentTime() const { return m_CurrentTime; }
        float GetDuration() const { return m_Duration; }
        void SetDuration(float duration) { m_Duration = duration; }

    protected:
        MovementMode m_Mode;
        float m_CurrentTime; // in seconds
        float m_Duration;    // in seconds
        float m_TimeSpeed = 1.0; // 1, -1, 0
    };
}
