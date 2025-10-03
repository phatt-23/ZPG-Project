#include "DirectionalLight.h"

namespace ZPG {

DirectionalLight::DirectionalLight(glm::vec4 const& color, glm::vec3 const& direction) 
: Light(LightType::Directional)
, ColorComponent(color)
, DirectionComponent(glm::normalize(direction)) {

}
void DirectionalLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) {
    std::string idx = std::to_string(index);
    shaderProgram.SetInt("u_Lights[" + idx + "].type", (i32)GetLightType());
    shaderProgram.SetFloat4("u_Lights[" + idx + "].color", GetColor());
    shaderProgram.SetFloat3("u_Lights[" + idx + "].dir", GetDirection());
}


}
