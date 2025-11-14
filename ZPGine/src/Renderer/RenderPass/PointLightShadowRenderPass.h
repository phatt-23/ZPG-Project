#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    class ShaderProgram;

    class PointLightShadowRenderPass : public RenderPass
    {
    public:
        PointLightShadowRenderPass();
        ~PointLightShadowRenderPass() override;

        void Init(RenderContext& renderContext) override;

        void Execute(RenderContext& renderContext) override;

    private:

        ref<ShaderProgram> m_ShaderProgram;
    };
}

