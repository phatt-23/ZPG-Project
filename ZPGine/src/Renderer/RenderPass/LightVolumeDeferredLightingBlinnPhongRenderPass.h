//
// Created by phatt on 11/18/25.
//

#pragma once

#include "Core/Core.h"
#include "Renderer/RenderPass.h"

namespace ZPG
{
    class Model;
    class VertexArray;

    class LightVolumeDeferredLightingBlinnPhongRenderPass : public RenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;

    private:
        void BindGeometryMaps(RenderContext& context);
        void BindLightMaps(RenderContext& context);

        ref<VertexArray> m_SphereVAO;
        ref<VertexArray> m_ConeVAO;
        ref<VertexArray> m_QuadVAO;

        ref<ShaderProgram> m_EnvironmentLightShaderProgram;
        ref<ShaderProgram> m_PointLightShaderProgram;
        ref<ShaderProgram> m_SpotLightShaderProgram;
    };

} // ZPG