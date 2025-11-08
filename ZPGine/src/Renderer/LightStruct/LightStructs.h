#pragma once

namespace ZPG
{
    class SpotLight;
    class PointLight;
    class DirectionalLight;
    class AmbientLight;

    struct DirectionalLightStruct
    {
        v4 Color;  // 16
        v3 Direction;
        f32 _pad0[1]{};  // 16
        m4 ViewProj;

    public:
        DirectionalLightStruct() = default;
        DirectionalLightStruct(const DirectionalLight& directionalLight, const v3& cameraPosition, const v3& cameraFront);
    };


    struct AmbientLightStruct 
    {
        v4 Color;   // 16
    public:
        AmbientLightStruct() = default;
        AmbientLightStruct(const AmbientLight& ambientLight);
    };

    struct PointLightStruct 
    {
        v4 Color;  // 16
        v3 Position;
        f32 _pad0[1];  // 16
        v3 Attenuation; 
        f32 _pad1[1];  // 16

    public:
        PointLightStruct() = default;
        PointLightStruct(const PointLight& pointLight);
    };

    struct SpotLightStruct 
    {
        v4 Color;  // 16
        v3 Position; 
        f32 _pad0[1];  // 16
        v3 Direction;
        f32 BeamSize;  // 16
        v3 Attenuation; 
        f32 BeamBlend; // 16
    
    public:
        SpotLightStruct() = default;
        SpotLightStruct(const SpotLight& spotlight);
    };
}
