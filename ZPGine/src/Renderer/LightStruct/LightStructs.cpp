#include "LightStructs.h"

#include "Camera/Camera.h"
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
            float brightnessCutoff = 5.0 / 256.0; // threshold
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

    DirectionalLightStruct::DirectionalLightStruct(const DirectionalLight& directionalLight, const Camera& camera)
        : Color(directionalLight.Color.Get())
        , Direction(directionalLight.Direction.Get())
    {
        ZPG_PROFILE_FUNCTION();

        f32 size = 40.0f;

        m4 lightView = glm::lookAt(-Direction, v3(0.0), v3(0, 1, 0));
        m4 lightProjection = glm::ortho(-size, size, -size, size, -size, size);

        ViewProj = lightProjection * lightView;
    }


}
