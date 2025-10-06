#pragma once
#include "Core/Core.h"
#include "LightType.h"
#include "Shader/ShaderProgram.h"

namespace ZPG {

class Light {
public:
    Light(LightType lightType) 
    : m_LightType(lightType) {
    }

    virtual ~Light() = default;

    LightType GetLightType() const { return m_LightType; }
    virtual void SendToShaderProgram(ShaderProgram& shaderProgram, u32 index) = 0;
private:
    LightType m_LightType;
};

}
