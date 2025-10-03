#pragma once

#include "Light.h"
#include "Light/ColorComponent.h"
#include "Light/PositionComponent.h"

namespace ZPG {

class PointLight : public Light, public ColorComponent, public PositionComponent {
public:
    PointLight(const glm::vec4& color, const glm::vec3& position);
    void SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) override;
private:
};

}
