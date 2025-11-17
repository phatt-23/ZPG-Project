#pragma once

#include "Renderer/RenderPass.h"

namespace ZPG
{
    /**
     * TODO: Use cascaded shadow mapping.
     * TODO: Single shadow map where viewproj matrix is changing based on the camera's front and position isn't cutting it.
     */
    class DirectionalLightShadowRenderPass : public RenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;

    private:
        ref<ShaderProgram> m_ShaderProgram;
    };
}
