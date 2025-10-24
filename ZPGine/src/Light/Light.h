#pragma once
#include "LightStruct.h"
#include "LightType.h"

namespace ZPG {

class Transform;
class ShaderProgram;

class Light {
public:
    Light(LightType lightType);
    virtual ~Light() = default;

    LightType GetLightType() const;
    virtual void SendToShaderProgram(ShaderProgram& shaderProgram, u32 index) = 0;
    virtual LightStruct MapToLightStruct() = 0;

protected:
    LightType m_LightType;
};

}
