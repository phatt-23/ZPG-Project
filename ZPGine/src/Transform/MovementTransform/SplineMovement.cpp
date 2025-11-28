#include "SplineMovement.h"

#include "Core/Math.h"
#include "Debug/Asserter.h"

namespace ZPG
{
    SplineSegment::SplineSegment(const v3& P1, const v3& P2, const v3& P3)
        : m_ControlPoints({ v3(0.0), P1, P2, P3 })
    {
        m_PresentPoints |= (1 << 1) | (1 << 2) | (1 << 3);
    }

    SplineSegment::SplineSegment(const v3& P0, const v3& P1, const v3& P2, const v3& P3)
        : m_ControlPoints({ P0, P1, P2, P3 })
    {
        m_PresentPoints |= (1 << 0) | (1 << 1) | (1 << 2) | (1 << 3);
    }

    const std::array<v3, 4>& SplineSegment::GetControlPoints() const
    {
        return m_ControlPoints;
    }

    f32 SplineSegment::GetLength()
    {
        if (m_Length != -1)
        {
            return m_Length;
        }

        const v3& P0 = m_ControlPoints[0];
        const v3& P1 = m_ControlPoints[1];
        const v3& P2 = m_ControlPoints[2];
        const v3& P3 = m_ControlPoints[3];

        const float approxArcLength = 0.5f * ( glm::distance(P0, P3) + glm::distance(P0, P1) + glm::distance(P1, P2) + glm::distance(P2, P3) );

        m_Length = approxArcLength;
        return m_Length;
    }

    const v3& SplineSegment::GetStartPoint() const
    {
        return m_ControlPoints[0];
    }

    const v3& SplineSegment::GetEndPoint() const
    {
        return m_ControlPoints[3];
    }

    void SplineSegment::SetStartPoint(const v3& startPoint)
    {
        m_ControlPoints[0] = startPoint;
        m_PresentPoints |= (1 << 0);
    }

    bool SplineSegment::IsPointPresent(int i) const
    {
        return (m_PresentPoints & (1 << i));
    }

    SplineMovement::SplineMovement(const v3& startPoint,
                                   std::initializer_list<SplineSegment>&& splineSegments,
                                   float duration,
                                   MovementMode mode)
        : Movement(duration, mode)
        , m_StartPoint(startPoint)
        , m_SplineSegments(splineSegments)
    {
        Invalidate();
    }

    SplineMovement::SplineMovement(const v3& startPoint, 
                                   const std::vector<SplineSegment>& splineSegments, 
                                   float duration, 
                                   MovementMode mode)
        : Movement(duration, mode)
        , m_StartPoint(startPoint)
        , m_SplineSegments(splineSegments)
    {
        Invalidate();
    }

    void SplineMovement::Invalidate()
    {
        ZPG_CORE_ASSERT(!m_SplineSegments.empty());

        m_SplineSegmentLengths.resize(m_SplineSegments.size());
        m_TotalLength = 0;

        if (m_SplineSegments.size() < 2)
        {
            if (!m_SplineSegments[0].IsPointPresent(0))
            {
                m_SplineSegments[0].SetStartPoint(m_StartPoint);
            }

            m_TotalLength = m_SplineSegments[0].GetLength();
            m_SplineSegmentLengths = { m_TotalLength };
            return;
        }

        for (int i = 0; i < m_SplineSegments.size(); i++)
        {
            // first segment
            if (i == 0)
            {
                if (!m_SplineSegments[i].IsPointPresent(0))
                {
                    m_SplineSegments[i].SetStartPoint(m_StartPoint);
                }

                m_SplineSegmentLengths[i] = m_SplineSegments[i].GetLength();
            }
            else
            {
                if (!m_SplineSegments[i].IsPointPresent(0))
                {
                    m_SplineSegments[i].SetStartPoint(m_SplineSegments[i - 1].GetEndPoint());
                }

                m_SplineSegmentLengths[i] = m_SplineSegments[i].GetLength();
            }

            m_TotalLength += m_SplineSegmentLengths[i];
        }
    }

    v3 SplineMovement::GetCurrentPosition()
    {
        auto [P0, P1, P2, P3, t] = GetCurrentControlPoints();
        return Math::Bezier3(P0, P1, P2, P3, t);
    }

    v3 SplineMovement::GetCurrentHeading()
    {
        auto [P0, P1, P2, P3, t] = GetCurrentControlPoints();
        v3 tangent = Math::TangentBezier3(P0, P1, P2, P3, t);
        if (GetSpeedPerSecond() < 0) {
            tangent = -tangent;
        }
        return tangent;
    }

    void SplineMovement::Update(Timestep& ts)
    {
        Movement::Update(ts);

        v3 currentPoint = GetCurrentPosition();
        v3 currentHeading = GetCurrentHeading();

        

        m4 R = Math::RotationMatFromDirection(currentHeading);
        m4 T = glm::translate(m4(1.0f), currentPoint);

        m_Matrix = T * R;
    }

    SplineMovement::CurrentParameters SplineMovement::GetCurrentControlPoints()
    {
        float t = GetNormalizedTime();
        float target = t * m_TotalLength;
        float accum = 0.0f;

        for (int i = 0; i < m_SplineSegments.size(); i++)
        {
            float segLen = m_SplineSegmentLengths[i];
            if (target < accum + segLen)
            {
                const auto& spline = m_SplineSegments[i];
                const auto& cp = spline.GetControlPoints();
                const v3& start = cp[0];
                const v3& c1 = cp[1];
                const v3& c2 = cp[2];
                const v3& end = cp[3];

                float localT = (target - accum) / segLen;
                return { start, c1, c2, end, localT };
            }
            accum += segLen;
        }

        return { m_StartPoint, m_StartPoint, m_StartPoint, m_StartPoint, 0.0f };
    }
}
