#include "EnvironmentLightRenderPass.h"
#include "Buffer/VertexArray.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Shader/ShaderProgram.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/Shader.h"
#include "Renderer/RenderCommand.h"

namespace ZPG
{

    void EnvironmentLightRenderPass::Init(RenderContext &context)
    {
        m_FrameBuffer = context.Targets.MainFrameBuffer;


        m_ShaderProgram = ShaderProgram::Create("environment_light.program",
        {
            Shader::Create("./assets/shaders/multipass/light-volume/EnvironmentLight.vert"),
            Shader::Create("./assets/shaders/multipass/light-volume/EnvironmentLight.frag"),
        });

        m_ShaderProgram->Bind();
        m_ShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_POSITION_MAP, 
                                RenderBindingPoints::GEOMETRY_POSITION_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_NORMAL_MAP, 
                                RenderBindingPoints::GEOMETRY_NORMAL_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_ALBEDO_METALLIC_MAP, 
                                RenderBindingPoints::GEOMETRY_ALBEDO_METALLIC_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_EMISSIVE_ROUGHNESS_MAP, 
                                RenderBindingPoints::GEOMETRY_EMISSIVE_ROUGHNESS_MAP);
        m_ShaderProgram->SetInt(CommonShaderUniforms::DIRECTIONAL_LIGHT_SHADOW_MAP, 
                                RenderBindingPoints::DIRECTIONAL_LIGHT_SHADOW_MAP);
        m_ShaderProgram->Unbind();
    }
    
    void EnvironmentLightRenderPass::Execute(RenderContext &context)
    {
        m_FrameBuffer->Bind();

        // additive blending
        glEnable(GL_BLEND); 
        glBlendFunc(GL_ONE, GL_ONE);       
        glBlendEquation(GL_FUNC_ADD);
        // disable writing to depth buffer
        glDepthMask(GL_FALSE);
        
        BindGeometryMaps(context);
        BindLightMaps(context);

        m_ShaderProgram->Bind();
        m_QuadVAO->Bind();
        RenderCommand::Draw(*m_QuadVAO);
        m_QuadVAO->Unbind();
        m_ShaderProgram->Unbind();

        // restore
        glDisable(GL_BLEND);
        glDepthMask(GL_TRUE);

        m_FrameBuffer->Unbind();

    }

}
