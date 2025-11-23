#include "BezierMovement.h"

namespace ZPG
{

    BezierMovement::BezierMovement(const vec<v3>& controlPoints, float duration, MovementMode mode)
        : Movement(duration, mode)    
        , m_ControlPoints(controlPoints) 
    {
    }

    BezierMovement::~BezierMovement() 
    {   
    }

    static float binom(int n, int k)
    {
        float r = 1.0f;
        for (int i = 1; i <= k; i++)
        {
            r = r * (n - (k - i)) / float(i);
        }
        return r;
    }

    v3 BezierMovement::GetCurrentPosition() 
    {
        int n = m_ControlPoints.size() - 1;  // degree
        float t = m_Elapsed / m_Duration;

        v3 accum(0.0f);

        for (int i = 0; i <= n; i++)
        {
            float b = binom(n, i) * pow(1 - t, float(n - i)) * pow(t, float(i));
            accum += b * m_ControlPoints[i];
        }

        return accum;
    }

    void BezierMovement::Update(Timestep &ts)
    {
        Movement::Update(ts);
        v3 currentPoint = GetCurrentPosition();
        m_Matrix = glm::translate(m4(1.0f), currentPoint);
    }


}
