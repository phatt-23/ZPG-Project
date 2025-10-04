#include "PointLight.h"
#include "Light/LightArrayUniformNamer.h"

namespace ZPG {

PointLight::PointLight(const glm::vec4& color, const glm::vec3& position) 
: Light(LightType::Point), ColorComponent(color), PositionComponent(position) {
}
void PointLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) {
    shaderProgram.SetInt(   LightArrayUniformNamer::TypeUniName(index), (i32)GetLightType());
    shaderProgram.SetFloat4(LightArrayUniformNamer::ColorUniName(index), GetColor());
    shaderProgram.SetFloat3(LightArrayUniformNamer::PositionUniName(index), GetPosition());
}

}
