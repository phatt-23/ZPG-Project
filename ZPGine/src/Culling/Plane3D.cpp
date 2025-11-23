#include "Plane3D.h"

namespace ZPG
{
    Plane3D::Plane3D()
        : m_Normal(0,0,0)
        , m_D(0)
    {
    }


    // Correctness was proven via Desmos visualisation.
    Plane3D::Plane3D(const v3& p0, const v3& p1, const v3& p2)
    {
        m_Normal = glm::normalize(glm::cross((p1 - p0), (p2 - p0)));

        /**
        *       ^ N(0,1,0)
        *       |
        *     /-|---* P(1,1,0)
        *   d | |  /
        *     | | /
        *     |_|/
        *       origin
        *
        *  I want to know the distance d, which is just:
        *  d = dot(N, P) = magnitude(N) * magnitude(P) * cos(angle)
        *
        *  We know that magnitude(N) = 1, therefore:
        *  d = magnitude(P) * cos(angle)
        *
        *  The magnitude(P) is greater than d, 
        *  but since P is angled away from N (the angle is 45 degrees) 
        *  the cos(angle) will be sqrt(2)/2 = 0.707 which is less than 1.
        *
        * Therefore d = magnitude(P) * 0.707, not the whole magnitude(P).
        */

        m_D = -glm::dot(m_Normal, p0);
    }
       
    float Plane3D::SignedDistance(const v3& point) const
    {
        // Uses the same thing described in the constructor.
        return glm::dot(m_Normal, point) + m_D;
    }

    Halfspace Plane3D::ClassifyPoint(const v3& point) const
    {
        float dist = SignedDistance(point);
        if (dist > 0.f) {
            return Halfspace::Front;
        }
        if (dist < 0.f) {
            return Halfspace::Back;
        }
        return Halfspace::OnPlane;
    }

    bool Plane3D::IsInFrontHalfspace(const v3& point) const
    {
        return ClassifyPoint(point) == Halfspace::Front;
    }

    bool Plane3D::IsInBackHalfspace(const v3& point) const
    {
        return ClassifyPoint(point) == Halfspace::Back;
    }

    bool Plane3D::IsOnPlane(const v3& point) const
    {
        return ClassifyPoint(point) == Halfspace::OnPlane;
    }
}

