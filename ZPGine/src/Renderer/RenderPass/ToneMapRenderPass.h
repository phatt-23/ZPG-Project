#pragma once

#include "Renderer/RenderPass.h"
namespace ZPG
{
    /**
     * Applies gamma correction and exposure to the main framebuffer's 
     * main color attachment.
    */
    class ToneMapRenderPass : public RenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
    };
}
