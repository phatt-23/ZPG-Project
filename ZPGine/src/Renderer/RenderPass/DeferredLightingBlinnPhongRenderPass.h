//
// Created by phatt on 11/16/25.
//

#pragma once

#include "Renderer/RenderPass.h"
#include "Renderer/RenderPass/LightVolume/LightRenderPass.h"

namespace ZPG
{
    class VertexArray;

    class DeferredLightingBlinnPhongRenderPass : public LightRenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
    };
}
