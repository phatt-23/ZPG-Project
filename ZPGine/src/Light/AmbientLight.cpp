#include "AmbientLight.h"
#include <sstream>

namespace ZPG {


AmbientLight::AmbientLight(const glm::vec4& color) 
: Light(LightType::Ambient)
, ColorComponent(color) {
}

void AmbientLight::SendToShaderProgram(ShaderProgram& shaderProgram, u32 index) {
    std::string indexStr = std::to_string(index);
    // TODO: These don't have to be set
    shaderProgram.SetInt("u_Lights[" + indexStr + "].type", (i32)GetLightType());
    shaderProgram.SetFloat4("u_Lights[" + indexStr + "].color", GetColor());
    // This one does have to be set
    shaderProgram.SetFloat4("u_AmbientColor", GetColor());
}

}
