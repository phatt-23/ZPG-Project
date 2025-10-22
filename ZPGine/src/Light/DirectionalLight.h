#pragma once

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

    void SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) override;

public:
    ColorComponent m_Color;
    DirectionComponent m_Direction;

};

}
