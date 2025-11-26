#pragma once

#include "Camera/Camera.h"
#include "Culling/Plane3D.h"

namespace ZPG
{
    class SphereVolume;
    class AABB;

    enum FrustumHit
    {
        FrustumHitOutside = 0,
        FrustumHitInside = 1,
        FrustumHitIntersect = 2,
    };


    class Frustum
    {   
    public:
        enum Sides 
        {
            Near = 0, 
            Far = 1, 
            Left = 2, 
            Right = 3, 
            Top = 4, 
            Bottom = 5,
        };

        enum Corner
        {
            NearTopRight,   
            NearTopLeft,
            NearBottomRight, 
            NearBottomLeft,
            FarTopRight,   
            FarTopLeft,
            FarBottomRight, 
            FarBottomLeft,
        };

        Frustum();
        ~Frustum();
        
        void Set(const Camera& camera);
        void Set(const m4& viewMatrix, const m4& projMatrix);

        FrustumHit ClassifyPoint(const v3& point) const;

        const std::array<v3, 8>& GetCorners() const { return m_Corners; }
        const std::array<Plane3D, 6>& GetPlanes() const { return m_Planes; }

    private:
        std::array<Plane3D, 6> m_Planes;
        std::array<v3, 8> m_Corners;
    };
}
