#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
class ShaderProgram;

class SpotLightShadowRenderPass : public RenderPass
    {
    public:
        SpotLightShadowRenderPass();
        ~SpotLightShadowRenderPass() override;
            
        void Init(RenderContext &renderContext) override;
        void Execute(RenderContext &renderContext) override;

    private:
        ref<ShaderProgram> m_ShaderProgram;
        u32 m_ShadowMapArrayRendererID;
    };

}
