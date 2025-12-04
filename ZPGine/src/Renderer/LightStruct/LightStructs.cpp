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
            float brightnessCutoff = 0.05; // threshold
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
        , Intensity(spotlight.Atten.GetIntensity())
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
        , Intensity(pointLight.Atten.GetIntensity())
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
            float sliceNear = camNear;
            float sliceFar  = zDistance + zStep;
            m4 frustumProj  = glm::perspective(glm::radians(fov), ar, sliceNear, sliceFar);
            zDistance += zStep;

            Frustum frustum;
            frustum.Set(frustumViewMatrix, frustumProj);
            const auto& corners = frustum.GetCorners();

            // compute center of the frustum slice in world space
            v3 center(0.0f);
            for (const auto& corner : corners) {
                center += corner;
            }
            center /= corners.size();

            // build directional light view matrix looking at the slice
            m4 lightView = glm::lookAt(
                center - Direction,
                center,
                v3(0,1,0)
            );

            // compute AABB of corners in light space
            v3 minL(std::numeric_limits<f32>::max());
            v3 maxL(std::numeric_limits<f32>::lowest());

            for (const auto& corner : corners)
            {
                v4 ls = lightView * v4(corner, 1.0f);
                v3 l  = v3(ls) / ls.w;
                minL.x = std::min(minL.x, l.x);
                minL.y = std::min(minL.y, l.y);
                minL.z = std::min(minL.z, l.z);
                maxL.x = std::max(maxL.x, l.x);
                maxL.y = std::max(maxL.y, l.y);
                maxL.z = std::max(maxL.z, l.z);
            }

            float depthRange = maxL.z - minL.z;
            float zPad = depthRange * 0.2f;   

            // expand depth in light-space Z back and forward
            minL.z -= zPad;
            maxL.z += zPad;

            m4 lightProj = glm::ortho( minL.x, maxL.x, minL.y, maxL.y, minL.z, maxL.z );

            PlaneDistance[i] = sliceFar;
            ViewProj[i] = lightProj * lightView;
        }
    }


}
