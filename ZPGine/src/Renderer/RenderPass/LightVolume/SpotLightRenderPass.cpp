#include "SpotLightRenderPass.h"
#include "Buffer/VertexArray.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Shader/Shader.h"
#include "Shader/ShaderProgram.h"
#include "Shader/CommonShaderUniforms.h"
#include "Renderer/RenderCommand.h"
#include "Transform/TransformGroup.h"
#include "Transform/StaticTransform/Rotate.h"
#include "Transform/StaticTransform/Scale.h"
#include "Transform/StaticTransform/Translate.h"

namespace ZPG
{
    void SpotLightRenderPass::Init(RenderContext& context) 
    {
        m_FrameBuffer = context.Targets.MainFrameBuffer;


        m_ShaderProgram = ShaderProgram::Create("spot_light.program",
        {
            Shader::Create("./assets/shaders/multipass/light-volume/SpotLight.vert"),
            Shader::Create("./assets/shaders/multipass/light-volume/SpotLight.frag"),
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
        m_ShaderProgram->SetInt(CommonShaderUniforms::SPOTLIGHT_SHADOW_MAP_ARRAY, 
                                RenderBindingPoints::SPOTLIGHT_SHADOW_MAP_ARRAY);
        m_ShaderProgram->Unbind();
    }

    void SpotLightRenderPass::Execute(RenderContext& context) 
    {
        // spotlight
        m_FrameBuffer->Bind();
        m_ShaderProgram->Bind();
        m_ShaderProgram->SetFloat2("u_ScreenSizeInv", 
            1.0f / v2(m_FrameBuffer->GetSpecification().Width, m_FrameBuffer->GetSpecification().Height));
        m_ConeVAO->Bind();
        
        BindGeometryMaps(context);
        BindLightMaps(context);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);       
        glBlendEquation(GL_FUNC_ADD);
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);

        // wireframe rendering
        // glDisable(GL_CULL_FACE);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        int index = 0;
        for (const auto& light : context.Lights.SpotLights)
        {
            // radius = height * tan(beamSize)
            // height is normalized, because the cone model im using is bounded by a 1x1x1 box
            // this cone's 'beam size' is 26 degrees
            // therefore, scale xz is calculated: tan(beamSize) / tan(26)

            static const float beamAngleDefault = tan(glm::radians(26.0f));
            float scaleXZ = tan(glm::radians(light.BeamSize)) / beamAngleDefault;

            auto model = TransformGroup::Build()
                .Add<Scale>(scaleXZ, 1.0f, scaleXZ)
                .Add<Rotate>(90.0f, v3(1.0, 0.0, 0.0))
                .Add<Rotate>(light.Direction)
                .Add<Scale>(light.Radius)
                .Add<Translate>(light.Position)
                .Compose();

            m_ShaderProgram->SetMat4("u_Model", model->GetMatrix());
            m_ShaderProgram->SetInt("u_Index", index++);

            RenderCommand::Draw(*m_ConeVAO);
        }

        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);

        // restore from wireframe
        // glEnable(GL_CULL_FACE);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        m_ConeVAO->Unbind();
        m_ShaderProgram->Unbind();
        m_FrameBuffer->Unbind();
    }
}
