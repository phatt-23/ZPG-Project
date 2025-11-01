#include "PointLight.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {

PointLight::PointLight(
    const glm::vec4& color,
    const glm::vec3& position,
    const AttenComponent atten
)
: Light(LightType::Point)
, Color(ColorComponent(color))
, Position(PositionComponent(position))
, Atten(atten) {

}

PointLight::PointLight(
    const ColorComponent& color,
    const PositionComponent& position,
    const AttenComponent& atten)
: Light(LightType::Point)
, Color(color)
, Position(position)
, Atten(atten) {

}

void PointLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) {
    using un = CommonShaderUniforms::LightArray;

    shaderProgram.SetInt(un::Type(index), (i32)GetLightType());
    shaderProgram.SetFloat4(un::Color(index), Color.Get());
    shaderProgram.SetFloat3(un::Position(index), Position.Get());
}

LightStruct PointLight::MapToLightStruct() {
    LightStruct lightStruct{};
    lightStruct.Type = static_cast<i32>(GetLightType());
    lightStruct.Color = Color.Get();
    lightStruct.Pos = Position.Get();
    lightStruct.Atten = Atten.GetAttenuation();
    return lightStruct;
}

}
