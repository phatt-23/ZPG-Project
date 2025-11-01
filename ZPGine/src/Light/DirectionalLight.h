#pragma once

#include "AttenComponent.h"
#include "Light/ColorComponent.h"
#include "Light/DirectionComponent.h"
#include "Light/Light.h"

namespace ZPG {

class ShaderProgram;

class DirectionalLight : public Light {
public:
    DirectionalLight(
        v4 const& color,
        v3 const& direction);

    DirectionalLight(
        const ColorComponent& color,
        const DirectionComponent& direction);

    void SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) override;
    LightStruct MapToLightStruct() override;
public:
    ColorComponent Color;
    DirectionComponent Direction;
};

}
