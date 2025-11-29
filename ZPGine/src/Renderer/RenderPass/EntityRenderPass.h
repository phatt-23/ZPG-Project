//
// Created by phatt on 11/18/25.
//

#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    class VertexArray;

    class EntityRenderPass : public RenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
    };

} // ZPG
