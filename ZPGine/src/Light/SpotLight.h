#pragma once

#include "AttenComponent.h"
#include "Light/ColorComponent.h"
#include "Light/BeamShapeComponent.h"
#include "Light/DirectionComponent.h"
#include "Light/PositionComponent.h"
#include "Light/Light.h"

namespace ZPG 
{
    class ShaderProgram;

    class SpotLight : public Light 
    {
    public:
        SpotLight(v4 const& color, 
                  v3 const& position, 
                  v3 const& direction, 
                  f32 beamSize, 
                  f32 beamBlend, 
                  const AttenComponent& atten = AttenComponent());

        SpotLight(const ColorComponent& color, 
                  const PositionComponent& position, 
                  const DirectionComponent& direction, 
                  const BeamShapeComponent& beamShape, 
                  const AttenComponent& atten = AttenComponent());

        [[deprecated]]
        void SendToShaderProgram(ShaderProgram &shaderProgram, u32 index) override;

        [[deprecated]]
        LightStruct MapToLightStruct() override;

    public:
        ColorComponent Color;
        PositionComponent Position;
        DirectionComponent Direction;
        BeamShapeComponent BeamShape;
        AttenComponent Atten;
    };

}
