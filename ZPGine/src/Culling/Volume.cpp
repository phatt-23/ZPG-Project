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

    bool SphereVolume::IsInFrustum(const Frustum& frustum, Transform& modelTransform) const
    {
        m4 model = modelTransform.GetMatrix();
        
        // decompose scale vector from model matrix
        v3 scale = modelTransform.GetScaleVector();
        v4 position4 = modelTransform.GetMatrix() * v4(m_Center, 1.0);
        v3 position = v3(position4) / position4.w;

        float maxScale = std::max({ scale.x, scale.y, scale.z });

        SphereVolume worldSphere = SphereVolume(position, (maxScale * 0.5) * m_Radius);

        for (const auto& plane : frustum.GetPlanes())
        {
            // is positive, then it is on the front halfspace, must be at least greater than its -radius 
            if (plane.SignedDistance(worldSphere.m_Center) > -worldSphere.m_Radius)
            {
                return false;
            }
        }

        return true;
    }

    AABB::AABB(const v3& center, v3 dimensions)
        : m_Center(center)
        , m_Dimensions(dimensions)
    {
    }

    AABB::~AABB()
    {
    }

    bool AABB::IsInFrustum(const Frustum& frustum, Transform& modelTransform) const
    {
        

        return FrustumHitInside;
    }
}
