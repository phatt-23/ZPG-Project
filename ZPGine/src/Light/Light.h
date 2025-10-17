#pragma once
#include "LightType.h"

namespace ZPG {

class ShaderProgram;

class Light {
public:
    Light(LightType lightType);
    virtual ~Light() = default;

    LightType GetLightType() const;
    virtual void SendToShaderProgram(ShaderProgram& shaderProgram, u32 index) = 0;
private:
    LightType m_LightType;
};

}
