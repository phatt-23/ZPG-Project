#include "PointLight.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {

PointLight::PointLight(const glm::vec4& color, const glm::vec3& position) 
: Light(LightType::Point)
, m_Color(ColorComponent(color))
, m_Position(PositionComponent(position)) {
}

void PointLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) {
    using un = CommonShaderUniforms::LightArray;

    shaderProgram.SetInt(un::Type(index), (i32)GetLightType());
    shaderProgram.SetFloat4(un::Color(index), m_Color.GetColor());
    shaderProgram.SetFloat3(un::Position(index), m_Position.GetPosition());
}

LightStruct PointLight::MapToLightStruct() {
    LightStruct lightStruct;
    lightStruct.Type = (u32)GetLightType();
    lightStruct.Color = m_Color.GetColor();
    lightStruct.Pos = m_Position.GetPosition();
    return std::move(lightStruct);
}

}
