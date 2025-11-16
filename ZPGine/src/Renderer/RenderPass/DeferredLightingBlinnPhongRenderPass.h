//
// Created by phatt on 11/16/25.
//

#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    class VertexArray;

    class DeferredLightingBlinnPhongRenderPass : public RenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;

    private:
        void BindGeometryMaps(RenderContext& context);
        void BindLightMaps(RenderContext& context);

        ref<VertexArray> m_QuadVAO;
    };
}
