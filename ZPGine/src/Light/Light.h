#pragma once
#include "LightStruct.h"
#include "LightType.h"

namespace ZPG {

class Transform;
class ShaderProgram;

class Light {
public:
    Light(LightType lightType, const ref<Transform>& transform = nullptr);
    virtual ~Light() = default;

    LightType GetLightType() const;
    virtual void SendToShaderProgram(ShaderProgram& shaderProgram, u32 index) = 0;
    virtual LightStruct MapToLightStruct() = 0;

    void SetTransform(const ref<Transform>& transform);
    const ref<Transform>& GetTransform() const;

protected:
    LightType m_LightType;
    ref<Transform> m_Transform;
};

}
