#pragma once

#include "LightRenderPass.h"
#include "Renderer/RenderPass.h"

namespace ZPG
{
    class EnvironmentLightRenderPass : public LightRenderPass
    {
    public:
        void Init(RenderContext &context) override;
        void Execute(RenderContext &context) override;
    };
}
