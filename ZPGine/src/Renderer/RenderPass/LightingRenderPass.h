#pragma once

#include "Renderer/RenderPass.h"
#include "Renderer/RenderPass/LightVolume/LightRenderPass.h"

namespace ZPG
{
    class VertexArray;
    class TextureCubeMap;
    class Texture2D;

    class LightingRenderPass : public LightRenderPass 
    {
    public:
        LightingRenderPass();

        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;

    private:
    };
}
