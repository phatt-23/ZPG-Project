#include "AmbientLight.h"
#include "LightArrayUniformNamer.h"
#include <sstream>

namespace ZPG {


AmbientLight::AmbientLight(const glm::vec4& color) 
: Light(LightType::Ambient)
, ColorComponent(color) {
}

void AmbientLight::SendToShaderProgram(ShaderProgram& shaderProgram, u32 index) {
    // This one does have to be set
    shaderProgram.SetFloat4("u_AmbientColor", GetColor());

    // TODO: These don't have to be set
    shaderProgram.SetInt(LightArrayUniformNamer::TypeUniName(index), (i32)GetLightType());
    shaderProgram.SetFloat4(LightArrayUniformNamer::ColorUniName(index), GetColor());
}

}
