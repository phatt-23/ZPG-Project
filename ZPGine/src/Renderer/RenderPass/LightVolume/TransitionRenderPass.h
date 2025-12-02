#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    /**
     * Sits between geometry pass and lighting pass.
     * It clears the main framebuffer and copies in 
     * the depth buffer from the geometry buffer.
    */
    class TransitionRenderPass : public RenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
    };
}
