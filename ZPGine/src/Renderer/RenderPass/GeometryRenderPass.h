#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    class Texture2D;

    class GeometryRenderPass : public RenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
    };
}
