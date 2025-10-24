#include "SpotLight.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {

SpotLight::SpotLight(
    v4 const& color,
    v3 const& position,
    v3 const& direction,
    f32 beamSize,
    f32 beamBlend)
: Light (LightType::Spotlight)
, m_Color (ColorComponent(color))
, m_Position (PositionComponent(position))
, m_Direction (DirectionComponent(direction))
, m_BeamShape (BeamShapeComponent(beamSize, beamBlend)) {

}

void SpotLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) {
    using uniName = CommonShaderUniforms::LightArray;
    shaderProgram.SetInt(uniName::Type(index),(i32)GetLightType());
    shaderProgram.SetFloat4(uniName::Color(index), m_Color.GetColor());
    shaderProgram.SetFloat3(uniName::Direction(index), m_Direction.GetDirection());
    shaderProgram.SetFloat3(uniName::Position(index), m_Position.GetPosition());
    shaderProgram.SetFloat(uniName::BeamSize(index), m_BeamShape.GetSize());
    shaderProgram.SetFloat(uniName::BeamBlend(index), m_BeamShape.GetBlend());
}

LightStruct SpotLight::MapToLightStruct() {
    LightStruct lightStruct;
    lightStruct.Type = u32(GetLightType());
    lightStruct.Color = (m_Color.GetColor());
    lightStruct.Pos = (m_Position.GetPosition());
    lightStruct.Dir = (m_Direction.GetDirection());
    lightStruct.BeamSize = (m_BeamShape.GetSize());
    lightStruct.BeamBlend = (m_BeamShape.GetBlend());
    return std::move(lightStruct);
}

}
