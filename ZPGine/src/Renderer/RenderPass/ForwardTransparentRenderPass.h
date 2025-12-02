#pragma once

#include "Renderer/RenderPass/LightVolume/LightRenderPass.h"

namespace ZPG
{
    /**
     * Using forward rendering, it draws draw calls specified
     * as transparent while being submitted into the renderer.
    */
    class ForwardTransparentRenderPass : public LightRenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
         
    private:
    };
}
