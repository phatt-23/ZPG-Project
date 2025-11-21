#include "LineMovement.h"
#include "Core/Utility.h"


namespace ZPG
{
    LineMovement::LineMovement(v3 start, v3 end, float duration)
        : Movement(duration)
        , m_StartPoint(start)
        , m_EndPoint(end)
    {
    }

    LineMovement::~LineMovement() 
    {
    }

    void LineMovement::Update(Timestep& ts)
    {
        Movement::Update(ts);
        m_CurrentTime = Utility::Wrap(m_CurrentTime, 0.0f, m_Duration);

        float t = m_CurrentTime / m_Duration;

        v3 currentPoint = ((1.0f - t) * m_StartPoint) + (t * m_EndPoint);

        m_Matrix = glm::translate(m4(1.0f), v3(currentPoint));
    }
}
