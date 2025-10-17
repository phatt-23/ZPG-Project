#pragma once

#include "Light/ColorComponent.h"
#include "Light/ConeCutoffComponent.h"
#include "Light/DirectionComponent.h"
#include "Light/PositionComponent.h"
#include "Light/Light.h"

namespace ZPG {

class ShaderProgram;

class SpotLight : public Light
                , public ColorComponent
                , public PositionComponent
                , public DirectionComponent
                , public ConeCutoffComponent {
public:
    SpotLight(
        v4 const& color,
        v3 const& position,
        v3 const& direction,
        f32 innerCutoffAngle,
        f32 outerCutoffAngle);

    void SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) override;
private:
};

}
