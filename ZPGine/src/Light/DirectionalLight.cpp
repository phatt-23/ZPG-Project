#include "DirectionalLight.h"
#include "LightArrayUniformNamer.h"

namespace ZPG {

DirectionalLight::DirectionalLight(glm::vec4 const& color, glm::vec3 const& direction) 
: Light(LightType::Directional)
, ColorComponent(color)
, DirectionComponent(glm::normalize(direction)) {

}
void DirectionalLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) {
    shaderProgram.SetInt(LightArrayUniformNamer::TypeUniName(index), (i32)GetLightType());
    shaderProgram.SetFloat4(LightArrayUniformNamer::ColorUniName(index), GetColor());
    shaderProgram.SetFloat3(LightArrayUniformNamer::DirectionUniName(index), GetDirection());
}


}
