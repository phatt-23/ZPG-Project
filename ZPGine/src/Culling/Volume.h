#pragma once

#include "Culling/Frustum.h"
#include "Transform/Transform.h"

namespace ZPG
{
    class Transform;
    class Frustum;


    class Volume 
    {
    public:
        virtual ~Volume() = default;
        virtual bool IsInFrustum(const Frustum& frustum, Transform& modelTransform) const = 0;

    private:

    };


    class SphereVolume : public Volume
    {
    public:
        SphereVolume(const v3& center, float radius);
        virtual ~SphereVolume() override;
        bool IsInFrustum(const Frustum& frustum, Transform& modelTransform) const override;
    private:
        v3 m_Center;
        float m_Radius;
    };


    /**
    * Axis aligned bounding box
    */
    class AABB : public Volume
    {
    public:
        AABB(const v3& center, v3 dimensions);
        virtual ~AABB() override;
        bool IsInFrustum(const Frustum &frustum, Transform& modelTransform) const override;
    private:
        v3 m_Center;
        v3 m_Dimensions;
    };

}
