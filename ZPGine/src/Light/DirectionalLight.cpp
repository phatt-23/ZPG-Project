#include "DirectionalLight.h"

#include "Profiling/Instrumentor.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {

DirectionalLight::DirectionalLight(glm::vec4 const& color, glm::vec3 const& direction)
: Light(LightType::Directional)
, Color(ColorComponent(color))
, Direction(DirectionComponent(glm::normalize(direction))) {
    ZPG_PROFILE_FUNCTION();

}

DirectionalLight::DirectionalLight(const ColorComponent& color, const DirectionComponent& direction)
: Light(LightType::Directional)
, Color(color)
, Direction(direction){
    ZPG_PROFILE_FUNCTION();
}

void DirectionalLight::SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) {
    ZPG_PROFILE_FUNCTION();
    using un = CommonShaderUniforms::LightArray;

    shaderProgram.SetInt(un::Type(index), (i32)GetLightType());
    shaderProgram.SetFloat4(un::Color(index), Color.Get());
    shaderProgram.SetFloat3(un::Direction(index), Direction.Get());
}

LightStruct DirectionalLight::MapToLightStruct() {
    ZPG_PROFILE_FUNCTION();
    LightStruct lightStruct{};
    lightStruct.Type = static_cast<i32>(GetLightType());
    lightStruct.Color = Color.Get();
    lightStruct.Dir = Direction.Get();
    return lightStruct;
}

}
