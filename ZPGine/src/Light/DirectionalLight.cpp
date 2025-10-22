#include "DirectionalLight.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {

DirectionalLight::DirectionalLight(glm::vec4 const& color, glm::vec3 const& direction) 
: Light(LightType::Directional)
, m_Color(ColorComponent(color))
, m_Direction(DirectionComponent(glm::normalize(direction))) {

}
void DirectionalLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) {
    using un = CommonShaderUniforms::LightArray;

    shaderProgram.SetInt(un::Type(index), (i32)GetLightType());
    shaderProgram.SetFloat4(un::Color(index), m_Color.GetColor());
    shaderProgram.SetFloat3(un::Direction(index), m_Direction.GetDirection());
}


}
