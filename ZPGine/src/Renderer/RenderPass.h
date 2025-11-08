#pragma once

namespace ZPG
{
    class RenderContext;

    class RenderPass
    {
    public:
        virtual ~RenderPass() = default;

        virtual void Init(RenderContext& renderContext) {}        

        virtual void Execute(RenderContext& renderContext) = 0;

        virtual void OnResize(u32 width, u32 height) {}

        virtual void Flush(RenderContext& renderContext);
    };

}

