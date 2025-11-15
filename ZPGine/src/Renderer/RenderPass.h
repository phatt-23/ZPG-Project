#pragma once

namespace ZPG
{
    class RenderContext;

    class RenderPass
    {
    public:
        virtual ~RenderPass() = default;

        virtual void Init(RenderContext& context) {}

        virtual void Execute(RenderContext& context) = 0;

        virtual void OnResize(u32 width, u32 height) {}

        virtual void Flush(RenderContext& context);
    };

}

