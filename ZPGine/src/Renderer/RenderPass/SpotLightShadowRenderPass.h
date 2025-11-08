#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{

    class SpotLightShadowRenderPass : public RenderPass
    {
    public:
        SpotLightShadowRenderPass();
        ~SpotLightShadowRenderPass();
            
        void Init(RenderContext &renderContext) override;
        void Execute(RenderContext &renderContext) override;
    };

}
