#include "SpotLight.h"

namespace ZPG {

SpotLight::SpotLight(glm::vec4 const& color, glm::vec3 const& position, 
                     glm::vec3 const& direction, f32 innerCutoff, f32 outerCutoff) 
: Light(LightType::Spotlight)
, ColorComponent(color)
, PositionComponent(position)
, DirectionComponent(direction)
, ConeCutoffComponent(innerCutoff, outerCutoff){
}
void SpotLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) {
    std::string idx = std::to_string(index);
    shaderProgram.SetInt("u_Lights[" + idx + "].type", (i32)GetLightType());
    shaderProgram.SetFloat4("u_Lights[" + idx + "].color", GetColor());
    shaderProgram.SetFloat3("u_Lights[" + idx + "].dir", GetDirection());
    shaderProgram.SetFloat3("u_Lights[" + idx + "].pos", GetPosition());
    shaderProgram.SetFloat("u_Lights[" + idx + "].inCutoff", GetInnerCutoff());
    shaderProgram.SetFloat("u_Lights[" + idx + "].outCutoff", GetOuterCutoff());
}

}
