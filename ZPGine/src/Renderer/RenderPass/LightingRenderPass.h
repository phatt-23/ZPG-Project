#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
class VertexArray;
class TextureCubeMap;
    class Texture2D;

    class LightingRenderPass : public RenderPass 
    {
    public:
        LightingRenderPass();

        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;

    private:
        void BindSky(RenderContext& context);
        void BindGeometryMaps(RenderContext& context);
        void BindLightMaps(RenderContext& context);

        ref<TextureCubeMap> m_NullSkyboxCubeMap;
        ref<Texture2D> m_NullSkydomeMap;
        ref<VertexArray> m_QuadVAO;
    };
}
