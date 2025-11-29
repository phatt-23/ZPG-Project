#pragma once

namespace ZPG
{
    class ShaderProgram;
    class FrameBuffer;
    class RenderContext;
    class VertexArray;

    class RenderPass
    {
    public:
        RenderPass();
        virtual ~RenderPass() = default;

        virtual void Init(RenderContext& context) = 0;
        virtual void Execute(RenderContext& context) = 0;
        virtual void OnResize(u32 width, u32 height);

    protected:
        ref<FrameBuffer> m_FrameBuffer = nullptr;
        ref<ShaderProgram> m_ShaderProgram = nullptr;
        ref<VertexArray> m_ScreenQuadVAO = nullptr;
    };

}

