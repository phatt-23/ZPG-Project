#include "AmbientLight.h"

#include "Shader/CommonShaderUniforms.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {


AmbientLight::AmbientLight(const glm::vec4& color) 
: Light(LightType::Ambient)
, ColorComponent(color) {
}

void AmbientLight::SendToShaderProgram(ShaderProgram& shaderProgram, u32 index) {
    shaderProgram.SetFloat4(CommonShaderUniforms::AMBIENT_COLOR, GetColor());

    // todo: These don't have to be set
    shaderProgram.SetInt(CommonShaderUniforms::LightArray::Type(index), (i32)GetLightType());
    shaderProgram.SetFloat4(CommonShaderUniforms::LightArray::Color(index), GetColor());
}

}
