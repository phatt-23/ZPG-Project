#include "Frustum.h"
#include "Culling/Volume.h"
#include "Debug/Asserter.h"

namespace ZPG
{
    const static umap<Frustum::Corner, v3> NDCCorners{
        { Frustum::NearTopRight,        v3(1,1,-1) },
        { Frustum::NearTopLeft,         v3(-1,1,-1) },
        { Frustum::NearBottomRight,     v3(1,-1,-1) },
        { Frustum::NearBottomLeft,      v3(-1,-1,-1) },
        { Frustum::FarTopRight,         v3(1,1,1) },
        { Frustum::FarTopLeft,          v3(-1,1,1) },
        { Frustum::FarBottomRight,      v3(1,-1,1) },
        { Frustum::FarBottomLeft,       v3(-1,-1,1) },
    };

    Frustum::Frustum() 
    {
        for (int i = 0; i < m_Planes.size(); i++)
        {   
            m_Planes[i] = Plane3D();
        }
        
        for (int i = 0; i < m_Corners.size(); i++)
        {
            m_Corners[i] = v3(0.0);
        }
    }

    Frustum::~Frustum() 
    {
    }
   
    void Frustum::Set(const Camera& camera)
    {
        this->Set(camera.GetViewMatrix(), camera.GetProjMatrix());
    }

    void Frustum::Set(const m4& viewMatrix, const m4& projMatrix)
    {
        m4 viewProjI = glm::inverse(projMatrix * viewMatrix);

        std::array<v3, 6> corners;
        for (const auto& [k, v] : NDCCorners) {
            v4 cornerWorldSpace = viewProjI * v4(v, 1.0);
            m_Corners[k] = v3(cornerWorldSpace) / cornerWorldSpace.w;
        }

    #if 0
        float fov = camera.GetFOV();
        float aspectRatio = camera.GetAspectRatio();
        float farPlane = camera.GetZFar();
        float nearPlane = camera.GetZNear();

        v3 z = -glm::normalize(camera.GetFront());
        v3 x = glm::normalize(camera.GetRight());
        v3 y = glm::normalize(glm::cross(z, x));

        v3 pos = camera.GetPosition();
        
        // compute near and far centers
        v3 nearCenter = pos - z * nearPlane;
        v3 farCenter = pos - z * farPlane;

        // tangens = protilehla_odvesna / prilehla_odvesna
        //           /|
        //          / |
        //         /  |
        //        /   |
        //       /    |
        //      /     |
        //     /      | height
        //    /       |
        //   /        |
        //  / theta   |
        // /__________|
        //     distance
        //
        // tan(theta) = height / distance
        // height = distance * tan(theta)
        //
        // Thanks to triangle similarity:
        // height_x = distance_x * tan(theta)
        float tang = glm::tan(glm::radians(fov * 0.5f));  // take half

        float nearHeight = nearPlane * tang;
        float nearWidth  = nearHeight * aspectRatio;  // aspectRatio = width/height  -> width = height * aspectRatio

        float farHeight = farPlane * tang;
        float farWidth  = farHeight * aspectRatio;

        // compute 8 corners
        m_Corners[NearTopLeft]     = nearCenter + y * nearHeight - x * nearWidth;
        m_Corners[NearTopRight]    = nearCenter + y * nearHeight + x * nearWidth;
        m_Corners[NearBottomLeft]  = nearCenter - y * nearHeight - x * nearWidth;
        m_Corners[NearBottomRight] = nearCenter - y * nearHeight + x * nearWidth;

        m_Corners[FarTopLeft]      = farCenter + y * farHeight - x * farWidth;
        m_Corners[FarTopRight]     = farCenter + y * farHeight + x * farWidth;
        m_Corners[FarBottomLeft]   = farCenter - y * farHeight - x * farWidth;
        m_Corners[FarBottomRight]  = farCenter - y * farHeight + x * farWidth;
    #endif

        // create planes with INWARD-facing normals (pointing into the frustum)
        // winding order reversed from original to flip normals
        m_Planes[Sides::Near] = Plane3D(m_Corners[NearBottomLeft], m_Corners[NearTopLeft], m_Corners[NearTopRight]);
        m_Planes[Sides::Far] = Plane3D(m_Corners[FarBottomRight], m_Corners[FarTopRight], m_Corners[FarTopLeft]);

        m_Planes[Sides::Left] = Plane3D(m_Corners[FarBottomLeft], m_Corners[FarTopLeft], m_Corners[NearTopLeft]);
        m_Planes[Sides::Right] = Plane3D(m_Corners[FarTopRight], m_Corners[FarBottomRight], m_Corners[NearBottomRight]);

        m_Planes[Sides::Top] = Plane3D(m_Corners[FarTopLeft], m_Corners[FarTopRight], m_Corners[NearTopRight]);
        m_Planes[Sides::Bottom] = Plane3D(m_Corners[FarBottomLeft], m_Corners[NearBottomLeft], m_Corners[NearBottomRight]);
    }

    FrustumHit Frustum::ClassifyPoint(const v3& point) const 
    {
        for (const auto& plane : m_Planes)        
        {
            // With inward-facing normals: if distance < 0, point is behind plane (outside frustum)
            if (plane.IsInBackHalfspace(point))
            {
                return FrustumHitOutside;
            }
        }
        
        return FrustumHitInside;
    }

}

