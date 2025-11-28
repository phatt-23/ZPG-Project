#include "Volume.h"
#include "Debug/Asserter.h"

namespace ZPG
{
    SphereVolume::SphereVolume(const v3& center, float radius)
        : m_Center(center)
        , m_Radius(radius)
    {
    }

    SphereVolume::~SphereVolume()
    {
    }

    bool SphereVolume::IsInFrustum(const Frustum& frustum, Transform& modelTransform, float tolerance) const
    {
        m4 model = modelTransform.GetMatrix();
        
        // decompose scale vector from model matrix
        v3 scale = modelTransform.GetScaleVector();
        v4 position4 = modelTransform.GetMatrix() * v4(m_Center, 1.0);
        v3 position = v3(position4) / position4.w;

        float maxScale = std::max({ scale.x, scale.y, scale.z });

        SphereVolume worldSphere = SphereVolume(position, maxScale * m_Radius);

        for (const auto& plane : frustum.GetPlanes())
        {
            // is positive, then it is on the front halfspace, must be at least greater than its -radius 
            // otherwise if it's less then the -radius, it's out of the frustum completely
            if (plane.SignedDistance(worldSphere.m_Center) <= -(worldSphere.m_Radius + tolerance))
            {
                return false;
            }
        }

        return true;
    }

    AABB::AABB(const v3& center, float halfExtentX, float halfExtentY, float halfExtentZ)
        : m_Center(center)
        , m_HalfExtents(halfExtentX, halfExtentY, halfExtentZ)
    {
    }

    AABB::AABB(const v3& minBound, const v3& maxBound)
        : m_Center(0.5f * (maxBound + minBound))
        , m_HalfExtents(0.5f * (maxBound - minBound))
    {
    }

    AABB::~AABB()
    {
    }

    bool AABB::IsInFrustum(const Frustum& frustum, Transform& modelTransform, float tolerance) const
    {
        using namespace std;
        using namespace glm;

        v4 position4( modelTransform.GetMatrix() * glm::vec4(v3(m_Center), 1.f) );
        v3 position = v3(position4) / position4.w;

        // Scaled orientation
        v3 right = modelTransform.GetRightVector() * m_HalfExtents.x;
        v3 up    = modelTransform.GetUpVector()    * m_HalfExtents.y;
        v3 front = modelTransform.GetFrontVector() * m_HalfExtents.z;

        // leaving it like this for clarity
        float dimX = abs(right.x) + abs(up.x) + abs(front.x);
        float dimY = abs(right.y) + abs(up.y) + abs(front.y);
        float dimZ = abs(right.z) + abs(up.z) + abs(front.z);

        AABB worldAABB(position, dimX, dimY, dimZ);

        for (const auto& plane : frustum.GetPlanes())
        {
            float radius = dot(worldAABB.m_HalfExtents, glm::abs(plane.GetNormal()));

            if (plane.SignedDistance(worldAABB.m_Center) <= -(radius + tolerance))
            {
                return false;
            }
        }

        return true;
    }
}
