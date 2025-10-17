#include "DirectionalLight.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {

DirectionalLight::DirectionalLight(glm::vec4 const& color, glm::vec3 const& direction) 
: Light(LightType::Directional)
, ColorComponent(color)
, DirectionComponent(glm::normalize(direction)) {

}
void DirectionalLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) {
    shaderProgram.SetInt(CommonShaderUniforms::LightArray::Type(index), (i32)GetLightType());
    shaderProgram.SetFloat4(CommonShaderUniforms::LightArray::Color(index), GetColor());
    shaderProgram.SetFloat3(CommonShaderUniforms::LightArray::Direction(index), GetDirection());
}


}
