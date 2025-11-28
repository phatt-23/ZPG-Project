#include "SplineMovement.h"

#include "Core/Math.h"
#include "Debug/Asserter.h"

namespace ZPG
{
    SplineSegment::SplineSegment(const v3& P2, const v3& P3)
        : m_ControlPoints({ v3(0), v3(0), P2, P3 })
    {
        m_PresentPoints |= (1 << 2) | (1 << 3);
    }

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

    f32 SplineSegment::ComputeLengthAccurate(int samples) const 
    {
        float length = 0.0f;
        const auto& cp = m_ControlPoints;

        v3 prev = cp[0];
        for (int i = 1; i <= samples; i++) 
        {
            float t = i / (float)samples;
            v3 current = Math::Bezier3(cp[0], cp[1], cp[2], cp[3], t);
            length += glm::distance(prev, current);
            prev = current;
        }
        return length;
    }

    f32 SplineSegment::GetLength()
    {
        if (m_Length >= 0)
        {
            return m_Length;
        }

        m_Length = ComputeLengthAccurate(32);
        return m_Length;

        // // approximation
        // const v3& P0 = m_ControlPoints[0];
        // const v3& P1 = m_ControlPoints[1];
        // const v3& P2 = m_ControlPoints[2];
        // const v3& P3 = m_ControlPoints[3];
        //
        // const float approxArcLength = 0.5f * ( glm::distance(P0, P3) + glm::distance(P0, P1) + glm::distance(P1, P2) + glm::distance(P2, P3) );
        //
        // m_Length = approxArcLength;
        // return m_Length;
    }

    const v3& SplineSegment::GetStartPoint() const
    {
        return m_ControlPoints[0];
    }

    const v3& SplineSegment::GetControlPoint1() const
    {
        return m_ControlPoints[1];
    }

    const v3& SplineSegment::GetControlPoint2() const 
    {
        return m_ControlPoints[2];
    }

    const v3& SplineSegment::GetEndPoint() const
    {
        return m_ControlPoints[3];
    }

    void SplineSegment::SetStartPoint(const v3& point)
    {
        SetPoint(0, point);
    }

    void SplineSegment::SetControlPoint1(const v3& point)
    {
        SetPoint(1, point);
    }

    void SplineSegment::SetPoint(int index, const v3& point)
    {
        ZPG_CORE_ASSERT(index >= 0 && index < m_ControlPoints.size());
        m_ControlPoints[index] = point;
        m_PresentPoints |= (1 << index);
    }

    bool SplineSegment::IsPointPresent(int i) const
    {
        return (m_PresentPoints & (1 << i));
    }

    f32 SplineSegment::ComputeTFromDistance(f32 targetDistance, int samples) 
    {
        const auto& cp = m_ControlPoints;
        float currentDistance = 0;
        float step = 1.0f / samples;

        // find which sample interval contains the distance
        v3 prevPoint = cp[0];
        for (i32 i = 1; i <= samples; i++) 
        {
            f32 t = i * step;
            v3 currentPoint = Math::Bezier3(cp[0], cp[1], cp[2], cp[3], t);
            f32 segmentDistance = glm::distance(prevPoint, currentPoint);

            if (currentDistance + segmentDistance >= targetDistance) 
            {
                // interpolate inside this tiny segment for better precision
                float excess = targetDistance - currentDistance;
                float ratio = excess / segmentDistance;
                float tPrev = (i - 1) * step;
                return glm::mix(tPrev, t, ratio);
            }

            currentDistance += segmentDistance;
            prevPoint = currentPoint;
        }

        return 1.0f;
    }




    SplineMovement::SplineMovement(const v3& startPoint,
                                   std::initializer_list<SplineSegment>&& splineSegments,
                                   float duration,
                                   MovementMode mode)
        : Movement(duration, mode)
        , m_StartPoint(startPoint)
        , m_Segments(splineSegments)
    {
        Invalidate();
    }

    SplineMovement::SplineMovement(const v3& startPoint, 
                                   const std::vector<SplineSegment>& splineSegments, 
                                   float duration, 
                                   MovementMode mode)
        : Movement(duration, mode)
        , m_StartPoint(startPoint)
        , m_Segments(splineSegments)
    {
        Invalidate();
    }

    void SplineMovement::Invalidate()
    {
        ZPG_CORE_ASSERT(!m_Segments.empty());

        m_SegmentLengths.resize(m_Segments.size());
        m_TotalLength = 0;

        for (int i = 0; i < m_Segments.size(); i++)
        {
            ZPG_CORE_ASSERT(m_Segments[i].IsPointPresent(2) && m_Segments[i].IsPointPresent(3));

            // first segment
            if (i == 0)
            {
                if (!m_Segments[i].IsPointPresent(1)) {
                    m_Segments[i].SetControlPoint1(m_Segments[i].GetControlPoint2());
                }

                if (!m_Segments[i].IsPointPresent(0)) {
                    m_Segments[i].SetStartPoint(m_StartPoint);
                }

                m_SegmentLengths[i] = m_Segments[i].GetLength();
            }
            else
            {
                if (!m_Segments[i].IsPointPresent(1)) {
                    m_Segments[i].SetControlPoint1(2.f * m_Segments[i - 1].GetEndPoint() - m_Segments[i - 1].GetControlPoint2());
                }

                if (!m_Segments[i].IsPointPresent(0)) {
                    m_Segments[i].SetStartPoint(m_Segments[i - 1].GetEndPoint());
                }

                m_SegmentLengths[i] = m_Segments[i].GetLength();
            }

            m_TotalLength += m_SegmentLengths[i];
        }
    }

    v3 SplineMovement::GetCurrentPosition()
    {
        auto [P0, P1, P2, P3, t] = GetCurrentBezier3Parameters();
        return Math::Bezier3(P0, P1, P2, P3, t);
    }

    v3 SplineMovement::GetCurrentHeading()
    {
        auto [P0, P1, P2, P3, t] = GetCurrentBezier3Parameters();
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

    SplineMovement::Bezier3Parameters SplineMovement::GetCurrentBezier3Parameters()
    {
        float t = GetNormalizedTime();
        float target = t * m_TotalLength;
        float accum = 0.0f;

        for (int i = 0; i < m_Segments.size(); i++)
        {
            float segLen = m_SegmentLengths[i];
            if (target < accum + segLen)
            {
                const auto& spline = m_Segments[i];
                const auto& cp = spline.GetControlPoints();
                const v3& start = cp[0];
                const v3& c1 = cp[1];
                const v3& c2 = cp[2];
                const v3& end = cp[3];

                // speed is varying by the length of a line, but corners nicely
                float localT = (target - accum) / segLen;  

                // consistent speed along tangents, but sharp and fast corners
                // float localT = m_Segs[i].ComputeTFromDistance(target - accum);  

                return { start, c1, c2, end, localT };
            }
            accum += segLen;
        }

        return { m_StartPoint, m_StartPoint, m_StartPoint, m_StartPoint, 0.0f };
    }

} // namespace ZPG
