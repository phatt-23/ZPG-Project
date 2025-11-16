#include "LightStructs.h"

#include "Camera/Camera.h"
#include "Light/SpotLight.h"
#include "Light/PointLight.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"
#include "Profiling/Instrumentor.h"

namespace ZPG
{
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

        ViewProj = proj * view;
    }

    PointLightStruct::PointLightStruct(const PointLight& pointLight)
        : Color(pointLight.Color.Get())
        , Position(pointLight.Position.Get())
        , Attenuation(pointLight.Atten.GetAttenuation())
    {
        // todo: base on attenuation
        f32 near = 0.1f;

        float quadratic = Attenuation.x;
        float linear = Attenuation.y;
        float constant = Attenuation.z;

        float lightMax = std::fmaxf(std::fmaxf(Color.r, Color.g), Color.b);
        float radius = (-linear + std::sqrtf(linear * linear - 4 * quadratic * (constant - (256.0 / 5.0) * lightMax))) / (2 * quadratic);

        Radius = radius;
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

    DirectionalLightStruct::DirectionalLightStruct(const DirectionalLight& directionalLight, const Camera& camera)
        : Color(directionalLight.Color.Get())
        , Direction(directionalLight.Direction.Get())
    {
        ZPG_PROFILE_FUNCTION();
#if 1
        f32 size = 40.0f;

        m4 lightView = glm::lookAt(-Direction, v3(0.0), v3(0, 1, 0));
        m4 lightProjection = glm::ortho(-size, size, -size, size, -size, size);

        ViewProj = lightProjection * lightView;
#endif
#if 0
        using namespace glm;

        float ar = camera.GetAspectRatio();
        float fov = radians(camera.GetFOV());
        float nearDist = 0.01f;
        float farDist = 100.0f;
        float Hnear = 2* tan(fov/2) * nearDist;
        float Wnear = Hnear * ar;
        float Hfar = 2* tan(fov/2) * farDist;
        float Wfar = Hfar * ar;
        vec3 centerFar = camera.GetPosition() + camera.GetFront() * farDist;

        vec3 topLeftFar = centerFar + (camera.GetWorldUp() * Hfar/2.0f) - (camera.GetRight() * Wfar/2.f);
        vec3 topRightFar = centerFar + (camera.GetWorldUp() * Hfar/2.0f) + (camera.GetRight() * Wfar/2.f);
        vec3 bottomLeftFar = centerFar - (camera.GetWorldUp()  * Hfar/2.0f) - (camera.GetRight() * Wfar/2.f);
        vec3 bottomRightFar = centerFar - (camera.GetWorldUp() * Hfar/2.0f) + (camera.GetRight() * Wfar/2.f);

        vec3 centerNear = camera.GetPosition() + camera.GetFront() * nearDist;

        vec3 topLeftNear = centerNear + (camera.GetWorldUp() * Hnear/2.f) - (camera.GetRight() * Wnear/2.f);
        vec3 topRightNear = centerNear + (camera.GetWorldUp() * Hnear/2.f) + (camera.GetRight() * Wnear/2.f);
        vec3 bottomLeftNear = centerNear - (camera.GetWorldUp() * Hnear/2.f) - (camera.GetRight() * Wnear/2.f);
        vec3 bottomRightNear = centerNear - (camera.GetWorldUp() * Hnear/2.f) + (camera.GetRight() * Wnear/2.f);

        vec3 frustumCenter = vec3(0.0f);
        for (const vec3& corner : { topLeftFar, topRightFar, bottomLeftFar, bottomRightFar,
                                     topLeftNear, topRightNear, bottomLeftNear, bottomRightNear }) {
            frustumCenter += corner;
        }
        frustumCenter /= 8.0f;

        vec3 lightDir = normalize(Direction); // e.g., vec3(-0.2f, -1.0f, -0.3f)
        vec3 lightPos = frustumCenter - lightDir * 50.0f; // move light back along its direction

        mat4 lightView = lookAt(lightPos, frustumCenter, vec3(0.0f, 1.0f, 0.0f));

        std::array frustumToLightView = {
            vec3(lightView * vec4(bottomRightNear, 1.0f)),
            vec3(lightView * vec4(topRightNear, 1.0f)),
            vec3(lightView * vec4(bottomLeftNear, 1.0f)),
            vec3(lightView * vec4(topLeftNear, 1.0f)),
            vec3(lightView * vec4(bottomRightFar, 1.0f)),
            vec3(lightView * vec4(topRightFar, 1.0f)),
            vec3(lightView * vec4(bottomLeftFar, 1.0f)),
            vec3(lightView * vec4(topLeftFar, 1.0f))
        };

        // find max and min points to define a ortho matrix around
        vec3 min(FLT_MAX), max(-FLT_MAX);
        for (const vec3& v : frustumToLightView) {
            min = glm::min(min, v);
            max = glm::max(max, v);
        }

        float zMult = 20.0f;
        min.z -= zMult;
        max.z += zMult;

        // finally, set our ortho projection
        // and create the light space view-projection matrix
        mat4 lightProjection = ortho(min.x, max.x, min.y, max.y, -max.z, -min.z);

        ViewProj = lightProjection * lightView;
#endif
#if 0
        const v3 camPos = camera.GetPosition();
        const v3 camFront = camera.GetFront();

        const float camNear = camera.GetZNear();
        const float camFar = camera.GetZFar();
        const float camFov = glm::radians(camera.GetFOV());
        const float camAspect = camera.GetAspectRatio();

        const float halfHeight = tan(camFov * 0.5f) * camFar;
        const float halfWidth = halfHeight * camAspect;
        const float size = std::max(halfWidth, halfHeight);

        const v3 lightDir = glm::normalize(Direction);
        const v3 target = camPos + camFront * (camFar * 0.5f);
        const v3 lightPos = target - lightDir * (camFar * 0.75f);

        const m4 view = glm::lookAt(lightPos, target, v3(0, 1, 0));
        const m4 proj = glm::ortho(-size, size, -size, size, camNear, camFar);

        ViewProj = proj * view;
#endif
    }


}
