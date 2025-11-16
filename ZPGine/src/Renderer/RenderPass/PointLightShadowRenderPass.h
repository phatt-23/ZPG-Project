#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    class PointLightShadowRenderPass : public RenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
    };
}

