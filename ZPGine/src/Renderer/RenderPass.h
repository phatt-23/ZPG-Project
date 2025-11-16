#pragma once

namespace ZPG
{
    class ShaderProgram;
    class FrameBuffer;
    class RenderContext;

    class RenderPass
    {
    public:
        virtual ~RenderPass() = default;

        virtual void Init(RenderContext& context) = 0;
        virtual void Execute(RenderContext& context) = 0;
        virtual void OnResize(u32 width, u32 height);

    protected:
        ref<FrameBuffer> m_FrameBuffer;
        ref<ShaderProgram> m_ShaderProgram;
    };

}

