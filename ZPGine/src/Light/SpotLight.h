#pragma once

#include "Light/ColorComponent.h"
#include "Light/BeamShapeComponent.h"
#include "Light/DirectionComponent.h"
#include "Light/PositionComponent.h"
#include "Light/Light.h"

namespace ZPG {

class ShaderProgram;

class SpotLight : public Light {
public:
    SpotLight(
        v4 const& color,
        v3 const& position,
        v3 const& direction,
        f32 beamSize,
        f32 beamBlend);

    void SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) override;
    LightStruct MapToLightStruct() override;

public:
    ColorComponent m_Color;
    PositionComponent m_Position;
    DirectionComponent m_Direction;
    BeamShapeComponent m_BeamShape;
};

}
