#pragma once

#include "Light/ColorComponent.h"
#include "Light/ConeCutoffComponent.h"
#include "Light/DirectionComponent.h"
#include "Light/PositionComponent.h"
#include "Light/Light.h"
namespace ZPG {

class SpotLight : public Light
                , public ColorComponent
                , public PositionComponent
                , public DirectionComponent
                , public ConeCutoffComponent {
public:
    SpotLight(glm::vec4 const& color, glm::vec3 const& position, glm::vec3 const& direction, 
              f32 innerCutoffAngle, f32 outerCutoffAngle);
    void SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) override;
private:
};

}
