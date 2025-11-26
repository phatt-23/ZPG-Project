#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    class VertexArray;
    class TextureCubeMap;
    class Texture2D;

    class LightRenderPass : public RenderPass
    {
    protected:
        LightRenderPass();
        virtual ~LightRenderPass();
        void BindGeometryMaps(const RenderContext &context);
        void BindLightMaps(const RenderContext &context);
        void BindSky(RenderContext& context);

        ref<VertexArray> m_SphereVAO;
        ref<VertexArray> m_QuadVAO;
        ref<VertexArray> m_ConeVAO;

        ref<TextureCubeMap> m_NullSkyboxCubeMap;
        ref<Texture2D> m_NullSkydomeMap;
    };
}
