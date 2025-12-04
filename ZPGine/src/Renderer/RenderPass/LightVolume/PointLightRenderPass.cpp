#include "PointLightRenderPass.h"
#include "Buffer/FrameBuffer.h"
#include "Buffer/VertexArray.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "Shader/CommonShaderUniforms.h"
#include "Renderer/RenderCommand.h"

namespace ZPG
{
    void PointLightRenderPass::Init(RenderContext& context) 
    {
        m_FrameBuffer = context.Targets.MainFrameBuffer;

        m_ShaderProgram = ShaderProgram::Create("point_light.program",
        {
            Shader::Create("./assets/shaders/multipass/light-volume/PointLight.vert"),
            Shader::Create("./assets/shaders/multipass/light-volume/PointLight.frag"),
        });

        m_ShaderProgram->Bind();
        m_ShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_POSITION_MAP, RenderBindingPoints::GEOMETRY_POSITION_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_NORMAL_MAP, RenderBindingPoints::GEOMETRY_NORMAL_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_ALBEDO_METALLIC_MAP, RenderBindingPoints::GEOMETRY_ALBEDO_METALLIC_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_EMISSIVE_ROUGHNESS_MAP, RenderBindingPoints::GEOMETRY_EMISSIVE_ROUGHNESS_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY, RenderBindingPoints::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY);
        m_ShaderProgram->Unbind();
    }

    void PointLightRenderPass::Execute(RenderContext& context) 
    {
        m_FrameBuffer->Bind();

        glDisable(GL_DEPTH_TEST);
        glDepthMask(GL_FALSE);
        //
        // // additive blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);       
        glBlendEquation(GL_FUNC_ADD);
        //
        // // render only the back face, otherwise the light doubles when both sides render
        // glEnable(GL_CULL_FACE);
        // glCullFace(GL_FRONT);


        // pointlight
        m_ShaderProgram->Bind();
        m_ShaderProgram->SetFloat2("u_ScreenSizeInv", 1.0f / m_FrameBuffer->GetDimensions());
        m_SphereVAO->Bind();

        int index = 0;
        for (const auto& light : context.Lights.PointLights)
        {
            m4 scale = glm::scale(m4(1.0), v3(light.Radius));
            m4 translate = glm::translate(m4(1.0), v3(light.Position));
            m4 model = translate * scale;

            m_ShaderProgram->SetMat4("u_Model", model);
            m_ShaderProgram->SetInt("u_Index", index++);

            RenderCommand::Draw(*m_SphereVAO);
        }

        // restore
        glEnable(GL_DEPTH_TEST);
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);

        m_SphereVAO->Unbind();
        m_ShaderProgram->Unbind();
        m_FrameBuffer->Unbind();
    }
}
