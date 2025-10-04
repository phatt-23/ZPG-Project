#include "SpotLight.h"
#include "LightArrayUniformNamer.h"

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
    std::string idx = std::to_string(index);
    shaderProgram.SetInt    (LightArrayUniformNamer::TypeUniName(index),       (i32)GetLightType());
    shaderProgram.SetFloat4 (LightArrayUniformNamer::ColorUniName(index),       GetColor());
    shaderProgram.SetFloat3 (LightArrayUniformNamer::DirectionUniName(index),   GetDirection());
    shaderProgram.SetFloat3 (LightArrayUniformNamer::PositionUniName(index),    GetPosition());
    shaderProgram.SetFloat  (LightArrayUniformNamer::InnerCutoffUniName(index), GetInnerCutoff());
    shaderProgram.SetFloat  (LightArrayUniformNamer::OuterCutoffUniName(index), GetOuterCutoff());
}

}
