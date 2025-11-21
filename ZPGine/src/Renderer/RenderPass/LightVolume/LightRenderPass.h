#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    class VertexArray;

    class LightRenderPass : public RenderPass
    {
    protected:
        LightRenderPass();
        void BindGeometryMaps(const RenderContext &context);
        void BindLightMaps(const RenderContext &context);

        ref<VertexArray> m_SphereVAO;
        ref<VertexArray> m_QuadVAO;
        ref<VertexArray> m_ConeVAO;
    };
}
