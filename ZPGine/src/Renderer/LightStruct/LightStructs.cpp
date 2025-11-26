#include "LightStructs.h"

#include "Camera/Camera.h"
#include "Culling/Frustum.h"
#include "Light/SpotLight.h"
#include "Light/PointLight.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"
#include "Profiling/Instrumentor.h"

namespace ZPG
{
    namespace Utils
    {
        float CalculateRadius(const v3& Color, const v3& Attenuation)
        {
            float quadratic = Attenuation.x;
            float linear = Attenuation.y;
            float constant = Attenuation.z;


            float lightMax = std::max({Color.r, Color.g, Color.b});
            float brightnessCutoff = 0.0001; // threshold
            float limit = lightMax / brightnessCutoff;

            float radius = (-linear + sqrt(linear * linear - 4 * quadratic * (constant - limit))) / (2 * quadratic);
            return radius;
        }
    }

    SpotLightStruct::SpotLightStruct(const SpotLight& spotlight)
        : Color(spotlight.Color.Get())  
        , Position(spotlight.Position.Get())
        , Direction(spotlight.Direction.Get())
        , BeamSize(spotlight.BeamShape.GetSize())
        , Attenuation(spotlight.Atten.GetAttenuation())
        , BeamBlend(spotlight.BeamShape.GetBlend())
    {
        f32 near = 0.1f;
        f32 far = 100.0f;
        f32 fov = glm::radians(2 * BeamSize);

        glm::vec3 up = glm::vec3(0, 1, 0);

        m4 view = glm::lookAt(Position, Position + Direction, up);
        m4 proj = glm::perspective(fov, 1.0f, near, far);

        Radius = Utils::CalculateRadius(Color, Attenuation);
        ViewProj = proj * view;
    }

    PointLightStruct::PointLightStruct(const PointLight& pointLight)
        : Color(pointLight.Color.Get())
        , Position(pointLight.Position.Get())
        , Attenuation(pointLight.Atten.GetAttenuation())
    {
        Radius = Utils::CalculateRadius(Color, Attenuation);

        // todo: base on attenuation
        f32 near = 0.1f;
        f32 aspect = 1.0f;
        m4 proj = glm::perspective(glm::radians(90.0f), aspect, near, Radius);

        ViewProj[0] = proj * glm::lookAt(Position, Position + v3(1,0,0), v3(0,-1,0)); // right
        ViewProj[1] = proj * glm::lookAt(Position, Position + v3(-1,0,0), v3(0,-1,0)); // left
        ViewProj[2] = proj * glm::lookAt(Position, Position + v3(0,1,0), v3(0,0,1)); // up
        ViewProj[3] = proj * glm::lookAt(Position, Position + v3(0,-1,0), v3(0,0,-1)); // down
        ViewProj[4] = proj * glm::lookAt(Position, Position + v3(0,0,1), v3(0,-1,0)); // back
        ViewProj[5] = proj * glm::lookAt(Position, Position + v3(0,0,-1), v3(0,-1,0)); // front
    }


    AmbientLightStruct::AmbientLightStruct(const AmbientLight& ambientLight)
        : Color(ambientLight.Color.Get()) 
    {

    }

    constexpr int DIRECTIONAL_LIGHT_SHADOW_CASCADE_COUNT = 4;

    DirectionalLightStruct::DirectionalLightStruct(const DirectionalLight& directionalLight, const Camera& camera)
        : Color(directionalLight.Color.Get())
        , Direction(directionalLight.Direction.Get())
        , CascadeCount(DIRECTIONAL_LIGHT_SHADOW_CASCADE_COUNT)
    {
        ZPG_PROFILE_FUNCTION();

        f32 fov = camera.GetFOV();
        f32 ar = camera.GetAspectRatio();
        f32 camNear = camera.GetZNear();
        f32 camFar = camera.GetZFar();
        
        const m4& frustumViewMatrix = camera.GetViewMatrix();

        f32 zDistance = camNear;
        f32 zStep = (camFar - camNear) / CascadeCount;

        for (int i = 0; i < CascadeCount; i++)
        {
            float near = zDistance;
            float far = zDistance + zStep;
            m4 frustumProjMatrix = glm::perspective(glm::radians(fov), ar, near, far);
            zDistance += zStep;

            Frustum frustum;
            frustum.Set(frustumViewMatrix, frustumProjMatrix);
            const auto& frustumCorners = frustum.GetCorners();
            
            // find the frustum's center and extents
            v3 frustumCenter(0.0);
            for (auto& corner : frustumCorners)
            {
                frustumCenter += corner;
            }
            frustumCenter /= frustumCorners.size();
                
            const m4 lightView = glm::lookAt(
                frustumCenter - Direction,
                frustumCenter,
                v3(0,1,0)
            );

            // frustum extents
            v3 maxExtents(std::numeric_limits<f32>::lowest());
            v3 minExtents(std::numeric_limits<f32>::max());
            for (auto& corner : frustumCorners)
            {
                v4 cornerLightSpace4(lightView * v4(corner, 1.0));
                v3 cornerLightSpace(cornerLightSpace4 / cornerLightSpace4.w);
                minExtents.x = std::min(cornerLightSpace.x, minExtents.x);
                minExtents.y = std::min(cornerLightSpace.y, minExtents.y);
                minExtents.z = std::min(cornerLightSpace.z, minExtents.z);
                maxExtents.x = std::max(cornerLightSpace.x, maxExtents.x);
                maxExtents.y = std::max(cornerLightSpace.y, maxExtents.y);
                maxExtents.z = std::max(cornerLightSpace.z, maxExtents.z);
            }

            /*
             If I didn't have such agressive frustum culling, 
             this would make sure to include some of the geometries 
             that are outside the frustum, because they are also 
             probably casting shadows into the visible scene.
            */

            constexpr float zMult = 4.0f;
            constexpr float xyMult = 2.0f;

            if (minExtents.z < 0) { minExtents.z *= zMult; }
            else                  { minExtents.z /= zMult; }
            if (maxExtents.z < 0) { maxExtents.z /= zMult; }
            else                  { maxExtents.z *= zMult; }

            if (minExtents.x < 0) { minExtents.x *= xyMult; }
            else                  { minExtents.x /= xyMult; }
            if (maxExtents.x < 0) { maxExtents.x /= xyMult; }
            else                  { maxExtents.x *= xyMult; }

            if (minExtents.y < 0) { minExtents.y *= xyMult; }
            else                  { minExtents.y /= xyMult; }
            if (maxExtents.y < 0) { maxExtents.y /= xyMult; }
            else                  { maxExtents.y *= xyMult; }

            const m4 lightProjection = glm::ortho(
                minExtents.x, maxExtents.x, 
                minExtents.y, maxExtents.y, 
                minExtents.z, maxExtents.z);

            PlaneDistance[i] = far;
            ViewProj[i] = lightProjection * lightView;
        }
    }


}
