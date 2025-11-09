#include "AmbientLight.h"

#include "Profiling/Instrumentor.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {


AmbientLight::AmbientLight(const glm::vec4& color) 
: Light(LightType::Ambient)
, Color(ColorComponent(color)) {
    ZPG_PROFILE_FUNCTION();
}

AmbientLight::AmbientLight(const ColorComponent& color)
: Light(LightType::Ambient)
, Color(color) {
    ZPG_PROFILE_FUNCTION();
}

void AmbientLight::SendToShaderProgram(ShaderProgram& shaderProgram, u32 index) {
    ZPG_PROFILE_FUNCTION();
    shaderProgram.SetFloat4(CommonShaderUniforms::AMBIENT_COLOR, Color.Get());

    // todo: These don't have to be set
    shaderProgram.SetInt(CommonShaderUniforms::LightArray::Type(index), (i32)GetLightType());
    shaderProgram.SetFloat4(CommonShaderUniforms::LightArray::Color(index), Color.Get());
}

LightStruct AmbientLight::MapToLightStruct() {
    ZPG_PROFILE_FUNCTION();
    LightStruct lightStruct{};
    lightStruct.Type = static_cast<i32>(GetLightType());
    lightStruct.Color = Color.Get();
    return lightStruct;
}

}
