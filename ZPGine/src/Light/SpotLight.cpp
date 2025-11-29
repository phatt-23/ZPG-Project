#include "SpotLight.h"

#include "Profiling/Instrumentor.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/ShaderProgram.h"

namespace ZPG 
{
    SpotLight::SpotLight(v4 const& color, 
                         v3 const& position, 
                         v3 const& direction, 
                         f32 beamSize, 
                         f32 beamBlend, 
                         const AttenComponent& atten)
        : Light (LightType::Spotlight)
        , Color (ColorComponent(color))
        , Position (PositionComponent(position))
        , Direction (DirectionComponent(direction))
        , BeamShape (BeamShapeComponent(beamSize, beamBlend))
        , Atten(atten)
    {
        ZPG_PROFILE_FUNCTION();
    }

    SpotLight::SpotLight(const ColorComponent& color, 
                         const PositionComponent& position, 
                         const DirectionComponent& direction, 
                         const BeamShapeComponent& beamShape, 
                         const AttenComponent& atten)
        : Light(LightType::Spotlight)
        , Color(color)
        , Position(position)
        , Direction(direction)
        , BeamShape(beamShape)
        , Atten(atten)
    {
        ZPG_PROFILE_FUNCTION();

    }

    void SpotLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) 
    {
        ZPG_PROFILE_FUNCTION();
        using uniName = CommonShaderUniforms::LightArray;
        shaderProgram.SetInt(uniName::Type(index),(i32)GetLightType());
        shaderProgram.SetFloat4(uniName::Color(index), Color.Get());
        shaderProgram.SetFloat3(uniName::Direction(index), Direction.Get());
        shaderProgram.SetFloat3(uniName::Position(index), Position.Get());
        shaderProgram.SetFloat(uniName::BeamSize(index), BeamShape.GetSize());
        shaderProgram.SetFloat(uniName::BeamBlend(index), BeamShape.GetBlend());
    }

    LightStruct SpotLight::MapToLightStruct() 
    {
        ZPG_PROFILE_FUNCTION();
        LightStruct lightStruct{};
        lightStruct.Type = static_cast<i32>(GetLightType());
        lightStruct.Color = Color.Get();
        lightStruct.Pos = Position.Get();
        lightStruct.Dir = Direction.Get();
        lightStruct.BeamSize = BeamShape.GetSize();
        lightStruct.BeamBlend = BeamShape.GetBlend();
        lightStruct.Atten = Atten.GetAttenuation();
        return lightStruct;
    }
}
