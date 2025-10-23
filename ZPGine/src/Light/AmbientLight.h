#pragma once

#include "Light.h"
#include "ColorComponent.h"

namespace ZPG {

class ShaderProgram;

class AmbientLight : public Light
{
public:
    AmbientLight(const v4& color);
    void SendToShaderProgram(ShaderProgram& shaderProgram, u32 index) override;
    LightStruct MapToLightStruct() override;
public:
    ColorComponent m_Color;
};

}

