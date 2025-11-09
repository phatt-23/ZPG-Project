#pragma once

#include "AttenComponent.h"
#include "Light.h"
#include "Light/ColorComponent.h"
#include "Light/PositionComponent.h"

namespace ZPG {

class PointLight : public Light
{
public:
    PointLight(
        const v4& color,
        const v3& position,
        const AttenComponent& atten = AttenComponent());

    PointLight(
        const ColorComponent& color,
        const PositionComponent& position,
        const AttenComponent& atten = AttenComponent());

    void SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) override;

    LightStruct MapToLightStruct() override;
public:
    ColorComponent Color;
    PositionComponent Position;
    AttenComponent Atten;
};

}
