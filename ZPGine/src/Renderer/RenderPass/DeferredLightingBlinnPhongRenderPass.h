//
// Created by phatt on 11/16/25.
//

#pragma once

#include "Renderer/RenderPass.h"
#include "Renderer/RenderPass/LightVolume/LightRenderPass.h"

namespace ZPG
{
    class VertexArray;

    /**
     * Deferred lighting pass that lights up the scene using 
     * the Blinn-Phong lighting model.
    */
    class DeferredLightingBlinnPhongRenderPass : public LightRenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
    };
}
