#include "Movement.h"
#include "Core/Timestep.h"
#include "Debug/Asserter.h"

namespace ZPG
{

    void Movement::Update(Timestep &ts)
    {
        m_Elapsed += m_TimeSpeed * ts.AsSeconds();

        switch (m_Mode)
        {
            case MovementMode::PingPong:
                if (m_Elapsed >= m_Duration) 
                {
                    m_TimeSpeed = -1.0f;
                }
                else if (m_Elapsed < 0)
                {
                    m_TimeSpeed = 1.0f;
                }
                break;
            case MovementMode::Repeat:
                if (m_Elapsed >= m_Duration)
                {
                    m_Elapsed = 0.0f;
                }
                break;
            case MovementMode::Once:
                if (m_Elapsed >= m_Duration)
                {
                    m_Elapsed = m_Duration;
                    m_TimeSpeed = 0.0f;
                }
                break;
            case MovementMode::Uncapped:
                break;
            case MovementMode::None:
            default:
                ZPG_UNREACHABLE("Invalid movement mode.");
        }
    } 

    Movement::Movement(float duration, MovementMode movementMode, float elapsed)
        : Transform()
        , m_Elapsed(elapsed)
        , m_Duration(duration) 
        , m_Mode(movementMode)
    {
    }

    Movement::~Movement() 
    {
    }

} // namespace ZPG
