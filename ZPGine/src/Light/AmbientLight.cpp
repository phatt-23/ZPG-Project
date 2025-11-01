#include "AmbientLight.h"

#include "Shader/CommonShaderUniforms.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {


AmbientLight::AmbientLight(const glm::vec4& color) 
: Light(LightType::Ambient)
, Color(ColorComponent(color)) {
}

AmbientLight::AmbientLight(const ColorComponent& color)
: Light(LightType::Ambient)
, Color(color) {
}

void AmbientLight::SendToShaderProgram(ShaderProgram& shaderProgram, u32 index) {
    shaderProgram.SetFloat4(CommonShaderUniforms::AMBIENT_COLOR, Color.Get());

    // todo: These don't have to be set
    shaderProgram.SetInt(CommonShaderUniforms::LightArray::Type(index), (i32)GetLightType());
    shaderProgram.SetFloat4(CommonShaderUniforms::LightArray::Color(index), Color.Get());
}

LightStruct AmbientLight::MapToLightStruct() {
    LightStruct lightStruct{};
    lightStruct.Type = static_cast<i32>(GetLightType());
    lightStruct.Color = Color.Get();
    return lightStruct;
}

}
