#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    class Texture2D;

    /**
     * Builds up the bloom texture by iteratively applying Gaussian blur
     * on the main framebuffer's main color attachment.
    */
    class BloomRenderPass : public RenderPass
    {
    public:
        virtual void Init(RenderContext& context) override;
        virtual void Execute(RenderContext& context) override;
        virtual void OnResize(u32 width, u32 height) override;

    private:
        ref<Texture2D> m_PingPongTexture;
        ref<FrameBuffer> m_PingPongFrameBuffer;
    };
};
