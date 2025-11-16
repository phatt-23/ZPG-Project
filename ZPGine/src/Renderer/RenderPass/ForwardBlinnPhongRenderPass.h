//
// Created by phatt on 11/15/25.
//
#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    class ForwardBlinnPhongRenderPass : public RenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
    };

}
