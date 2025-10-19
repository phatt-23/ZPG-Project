#pragma once

#include "Light.h"
#include "Light/ColorComponent.h"
#include "Light/PositionComponent.h"

namespace ZPG {

class PointLight
    : public Light
    , public ColorComponent
    , public PositionComponent {
public:
    PointLight(const v4& color, const v3& position);
    void SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) override;
private:
};

}
