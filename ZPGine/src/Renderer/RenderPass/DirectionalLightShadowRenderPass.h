#pragma once


#include "Renderer/RenderPass.h"
namespace ZPG
{
    class ShaderProgram;

    class DirectionalLightShadowRenderPass : public RenderPass
    {
    public:
        DirectionalLightShadowRenderPass();
        ~DirectionalLightShadowRenderPass() override;

        void Init(RenderContext &renderContext) override;
        void Execute(RenderContext &renderContext) override;

    private:
        ref<ShaderProgram> m_ShaderProgram;
    };
}
