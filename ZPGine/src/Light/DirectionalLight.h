#pragma once

#include "Light/ColorComponent.h"
#include "Light/DirectionComponent.h"
#include "Light/Light.h"

namespace ZPG {

class ShaderProgram;

class DirectionalLight : public Light, public ColorComponent, public DirectionComponent {
public:
    DirectionalLight(glm::vec4 const& color, glm::vec3 const& direction);
    void SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) override;
private:

};

}
