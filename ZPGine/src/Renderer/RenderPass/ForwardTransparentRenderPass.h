#pragma once

#include "Renderer/RenderPass/LightVolume/LightRenderPass.h"

namespace ZPG
{
    class ForwardTransparentRenderPass : public LightRenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
         
    private:
    };
}
