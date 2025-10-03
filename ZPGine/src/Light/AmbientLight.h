#pragma once


#include "Light.h"
#include "ColorComponent.h"

namespace ZPG {

class AmbientLight : public Light, public ColorComponent {
public:
    AmbientLight(const glm::vec4& color);
    virtual void SendToShaderProgram(ShaderProgram& shaderProgram, u32 index) override;
private:
};

}

