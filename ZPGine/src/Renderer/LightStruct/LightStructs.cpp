#include "LightStructs.h"
#include "Light/SpotLight.h"
#include "Light/PointLight.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"

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
        
    }

    PointLightStruct::PointLightStruct(const PointLight& pointLight)
        : Color(pointLight.Color.Get())
        , Position(pointLight.Position.Get())
        , Attenuation(pointLight.Atten.GetAttenuation())
    {
    }


    AmbientLightStruct::AmbientLightStruct(const AmbientLight& ambientLight)
        : Color(ambientLight.Color.Get()) 
    {

    }

    DirectionalLightStruct::DirectionalLightStruct(const DirectionalLight& directionalLight, const v3& cameraPosition, const v3& cameraFront)
        : Color(directionalLight.Color.Get())
        , Direction(directionalLight.Direction.Get())
    {
        f32 nearPlane = -100.0f;
        f32 farPlane = 100.0f;
        f32 size = 10.f;
        m4 proj = glm::ortho(-size, size, -size, size, nearPlane, farPlane);
        m4 view = glm::lookAt(-Direction, v3(0.0), v3(0, 1, 0));
        ViewProj = proj * view;
    }
}
