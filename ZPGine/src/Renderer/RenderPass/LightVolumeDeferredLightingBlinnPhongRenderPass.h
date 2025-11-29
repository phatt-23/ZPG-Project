//
// Created by phatt on 11/18/25.
//

#pragma once

#include "Core/Core.h"
#include "Renderer/RenderPass.h"
#include "Renderer/RenderPass/LightVolume/LightRenderPass.h"

namespace ZPG
{
    class Model;
    class VertexArray;

    class LightVolumeDeferredLightingBlinnPhongRenderPass : public LightRenderPass
    {
    public:
        void Init(RenderContext& context) override;
        void Execute(RenderContext& context) override;
        void OnResize(u32 width, u32 height) override;
    private:
        void UpdateUniforms();
    private:
        ref<ShaderProgram> m_EnvironmentLightShaderProgram;
        ref<ShaderProgram> m_PointLightShaderProgram;
        ref<ShaderProgram> m_SpotLightShaderProgram;
    };

} // ZPG
