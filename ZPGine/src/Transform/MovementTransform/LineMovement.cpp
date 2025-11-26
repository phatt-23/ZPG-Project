#include "LineMovement.h"

namespace ZPG
{
    LineMovement::LineMovement(const v3& start, const v3& end, float duration, MovementMode mode)
        : Movement(duration, mode)
        , m_StartPoint(start)
        , m_EndPoint(end)
    {
    }

    LineMovement::~LineMovement() 
    {
    }

    v3 LineMovement::GetCurrentPosition()
    {
        float t = GetNormalizedTime();
        v3 currentPoint = ((1.0f - t) * m_StartPoint) + (t * m_EndPoint);
        return currentPoint;
    }

    v3 LineMovement::GetCurrentHeading()
    {
        return m_EndPoint - m_StartPoint;
    }

    void LineMovement::Update(Timestep& ts)
    {
        Movement::Update(ts);
        v3 currentPoint = GetCurrentPosition();
        m_Matrix = glm::translate(m4(1.0f), currentPoint);
    }
}
