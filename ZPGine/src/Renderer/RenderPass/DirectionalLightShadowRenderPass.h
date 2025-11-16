#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    class DirectionalLightShadowRenderPass : public RenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;

    private:
        ref<ShaderProgram> m_ShaderProgram;
    };
}
