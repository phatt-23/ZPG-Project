#include "PolyLineMovement.h"

namespace ZPG
{
    PolyLineMovement::PolyLineMovement(const vec<v3> &points, 
                                       float duration, 
                                       MovementMode mode)
        : Movement(duration, mode)
        , m_Points(points) 
    {
        m_Lengths.reserve(m_Points.size() - 1);
        m_TotalLength = 0.0f;

        // Compute total length of the entire polyline
        for (int i = 0; i < m_Points.size() - 1; i++) 
        {
            float segment = glm::length(m_Points[i + 1] - m_Points[i]);
            m_Lengths.push_back(segment);
            m_TotalLength += segment;
        }
    }

    v3 PolyLineMovement::GetCurrentHeading() 
    {
        auto [i,j,t] = GetCurrentSegment();
        v3 heading = m_Points[j] - m_Points[i];
        return heading;
    }

    std::tuple<u32, u32, f32> PolyLineMovement::GetCurrentSegment()
    {
        float t = GetNormalizedTime();

        float target = t * m_TotalLength; // distance along the curve that corresponds to t

        float accum = 0.0f; // find which segment contains that distance
        for (int i = 0; i < m_Points.size() - 1; i++) 
        {
            float nextAccum = accum + m_Lengths[i];
            if (target <= nextAccum) 
            {
                // interpolation inside this segment
                float localT = (target - accum) / m_Lengths[i];
                return { i, i + 1, localT };
            }
            accum = nextAccum;
        }
        return { m_Points.size() - 2, m_Points.size() - 1, 1.0 };
    }

    v3 PolyLineMovement::GetCurrentPosition() 
    {
        if (m_Points.empty()) {
            return v3(0.0);
        }

        if (m_Points.size() < 2) {
            return m_Points[0];
        }

        auto [i, j, localT] = GetCurrentSegment();
        return glm::mix(m_Points[i], m_Points[j], localT);
    }

/*
 * previous implementation, i tried to abstract it, but theres a bug
    v3 PolyLineMovement::GetCurrentPosition()
    {
        if (m_Points.empty()) {
            return v3(0.0);
        }

        if (m_Points.size() < 2) {
            return m_Points[0];
        }

        float t = GetNormalizedTime();

        // distance along the curve that corresponds to t
        float target = t * m_TotalLength;

        // find which segment contains that distance
        float accum = 0.0f;
        for (int i = 0; i < m_Points.size() - 1; i++) 
        {
            float nextAccum = accum + m_Lengths[i];
            if (target <= nextAccum) 
            {
                // interpolation inside this segment
                float localT = (target - accum) / m_Lengths[i];
                return glm::mix(m_Points[i], m_Points[i + 1], localT);
            }
            accum = nextAccum;
        }

        return m_Points.back(); // fallback
    }
*/
    void PolyLineMovement::Update(Timestep &ts) 
    {
        Movement::Update(ts);
        v3 currentPoint = GetCurrentPosition();
        m_Matrix = glm::translate(m4(1.0f), currentPoint);
    }
}

