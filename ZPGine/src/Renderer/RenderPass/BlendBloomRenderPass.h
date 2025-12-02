#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    /**
     * Blends the bloom texture with the main framebuffer's 
     * main color attachment.
    */
    class BlendBloomRenderPass : public RenderPass
    {
    public:
        void Init(RenderContext &context) override;
        void Execute(RenderContext &context) override;
    private:
    };
}

