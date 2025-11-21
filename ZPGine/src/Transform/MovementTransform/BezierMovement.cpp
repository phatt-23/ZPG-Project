#include "BezierMovement.h"
#include "Core/Utility.h"
#include "Debug/Asserter.h"


namespace ZPG
{

    BezierMovement::BezierMovement(const vec<v3>& controlPoints, float duration)
        : Movement(duration), m_ControlPoints(controlPoints) {}

    BezierMovement::~BezierMovement() {}

    static float binom(int n, int k)
    {
        float r = 1.0f;
        for (int i = 1; i <= k; i++)
            r = r * (n - (k - i)) / float(i);
        return r;
    }

    void BezierMovement::Update(Timestep &ts)
    {
        Movement::Update(ts);
        m_CurrentTime = Utility::Wrap(m_CurrentTime, 0.0f, m_Duration);

        int n = m_ControlPoints.size() - 1;  // degree
        float t = m_CurrentTime / m_Duration;

        v3 accum(0.0f);

        for (int i = 0; i <= n; i++)
        {
            float b = binom(n, i) * pow(1 - t, float(n - i)) * pow(t, float(i));
            accum += b * m_ControlPoints[i];
        }

        m_Matrix = glm::translate(m4(1.0f), accum);
    }


}
