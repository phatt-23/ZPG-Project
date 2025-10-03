#include "PointLight.h"

namespace ZPG {

PointLight::PointLight(const glm::vec4& color, const glm::vec3& position) 
: Light(LightType::Point), ColorComponent(color), PositionComponent(position) {
}
void PointLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) {
    std::string idx = std::to_string(index);
    shaderProgram.SetInt("u_Lights[" + idx + "].type", (i32)GetLightType());
    shaderProgram.SetFloat4("u_Lights[" + idx + "].color", GetColor());
    shaderProgram.SetFloat3("u_Lights[" + idx + "].pos", GetPosition());
}

}
