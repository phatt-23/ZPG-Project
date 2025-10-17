#include "PointLight.h"
#include "Shader/CommonShaderUniforms.h"

namespace ZPG {

PointLight::PointLight(const glm::vec4& color, const glm::vec3& position) 
: Light(LightType::Point), ColorComponent(color), PositionComponent(position) {
}
void PointLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) {
    shaderProgram.SetInt(CommonShaderUniforms::LightArray::Type(index), (i32)GetLightType());
    shaderProgram.SetFloat4(CommonShaderUniforms::LightArray::Color(index), GetColor());
    shaderProgram.SetFloat3(CommonShaderUniforms::LightArray::Position(index), GetPosition());
}

}
