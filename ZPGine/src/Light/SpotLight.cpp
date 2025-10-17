#include "SpotLight.h"
#include "Shader/CommonShaderUniforms.h"

namespace ZPG {

SpotLight::SpotLight(glm::vec4 const& color, glm::vec3 const& position, 
                     glm::vec3 const& direction, f32 innerCutoffAngle, f32 outerCutoffAngle) 
: Light(LightType::Spotlight)
, ColorComponent(color)
, PositionComponent(position)
, DirectionComponent(direction)
, ConeCutoffComponent(innerCutoffAngle, outerCutoffAngle){
}
void SpotLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) {
    shaderProgram.SetInt(CommonShaderUniforms::LightArray::Type(index),(i32)GetLightType());
    shaderProgram.SetFloat4(CommonShaderUniforms::LightArray::Color(index), GetColor());
    shaderProgram.SetFloat3(CommonShaderUniforms::LightArray::Direction(index), GetDirection());
    shaderProgram.SetFloat3(CommonShaderUniforms::LightArray::Position(index), GetPosition());
    shaderProgram.SetFloat(CommonShaderUniforms::LightArray::InnerCutoff(index), GetInnerCutoff());
    shaderProgram.SetFloat(CommonShaderUniforms::LightArray::OuterCutoff(index), GetOuterCutoff());
}

}
