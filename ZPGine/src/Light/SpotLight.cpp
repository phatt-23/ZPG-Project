#include "SpotLight.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {

SpotLight::SpotLight(
    v4 const& color,
    v3 const& position,
    v3 const& direction,
    f32 innerCutoffAngle,
    f32 outerCutoffAngle)
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
