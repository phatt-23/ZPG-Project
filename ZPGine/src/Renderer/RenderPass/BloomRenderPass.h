#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    class Texture2D;

    class BloomRenderPass : public RenderPass
    {
    public:
        virtual void Init(RenderContext& context) override;
        virtual void Execute(RenderContext& context) override;
        virtual void OnResize(u32 width, u32 height) override;
    private:
        ref<ShaderProgram> m_BlendShaderProgram;
        ref<Texture2D> m_PingPongTexture;
        ref<FrameBuffer> m_PingPongFrameBuffer;
    };
};
