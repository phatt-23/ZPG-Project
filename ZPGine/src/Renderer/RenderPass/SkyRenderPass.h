#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    /**
     * Draws the sky, if some is active.
    */
    class SkyRenderPass : public RenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
    };

}
