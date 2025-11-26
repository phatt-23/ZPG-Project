//
// Created by phatt on 11/18/25.
//

#include "LightVolumeDeferredLightingBlinnPhongRenderPass.h"

#include "Renderer/LightStruct/LightStructs.h"
#include "Buffer/BufferLayout.h"
#include "Buffer/VertexArray.h"
#include "Model/Mesh.h"
#include "Model/Model.h"
#include "Model/ModelLoader.h"
#include "Profiling/Instrumentor.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderContext.h"
#include "Resource/CommonResources.h"
#include "Resource/ResourceManager.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/Shader.h"
#include "Shader/ShaderDataType.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"
#include "Texture/Texture2DArray.h"
#include "Texture/TextureCubeMapArray.h"
#include "Transform/TransformGroup.h"
#include "Transform/StaticTransform/Scale.h"
#include "Transform/StaticTransform/Rotate.h"
#include "Transform/StaticTransform/Translate.h"

namespace ZPG
{
    void LightVolumeDeferredLightingBlinnPhongRenderPass::Init(RenderContext& context)
    {
        ZPG_PROFILE_FUNCTION();

        m_ShaderProgram = nullptr;

        m_EnvironmentLightShaderProgram = ShaderProgram::Create("environment_light.program",
        {
            Shader::Create("./assets/shaders/multipass/light-volume/EnvironmentLight.vert"),
            Shader::Create("./assets/shaders/multipass/light-volume/EnvironmentLight.frag"),
        });

        m_EnvironmentLightShaderProgram->Bind();
        m_EnvironmentLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_POSITION_MAP, RenderBindingPoints::GEOMETRY_POSITION_MAP);
        m_EnvironmentLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_NORMAL_MAP, RenderBindingPoints::GEOMETRY_NORMAL_MAP);
        m_EnvironmentLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_ALBEDO_METALLIC_MAP, RenderBindingPoints::GEOMETRY_ALBEDO_METALLIC_MAP);
        m_EnvironmentLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_EMISSIVE_ROUGHNESS_MAP, RenderBindingPoints::GEOMETRY_EMISSIVE_ROUGHNESS_MAP);
        m_EnvironmentLightShaderProgram->SetInt(CommonShaderUniforms::DIRECTIONAL_LIGHT_SHADOW_MAP_ARRAY, RenderBindingPoints::DIRECTIONAL_LIGHT_SHADOW_MAP_ARRAY);
        m_EnvironmentLightShaderProgram->Unbind();

        m_PointLightShaderProgram = ShaderProgram::Create("point_light.program",
        {
            Shader::Create("./assets/shaders/multipass/light-volume/PointLight.vert"),
            Shader::Create("./assets/shaders/multipass/light-volume/PointLight.frag"),
        });
        m_PointLightShaderProgram->Bind();
        m_PointLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_POSITION_MAP, RenderBindingPoints::GEOMETRY_POSITION_MAP);
        m_PointLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_NORMAL_MAP, RenderBindingPoints::GEOMETRY_NORMAL_MAP);
        m_PointLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_ALBEDO_METALLIC_MAP, RenderBindingPoints::GEOMETRY_ALBEDO_METALLIC_MAP);
        m_PointLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_EMISSIVE_ROUGHNESS_MAP, RenderBindingPoints::GEOMETRY_EMISSIVE_ROUGHNESS_MAP);
        m_PointLightShaderProgram->SetInt(CommonShaderUniforms::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY, RenderBindingPoints::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY);
        m_PointLightShaderProgram->Unbind();

        m_SpotLightShaderProgram = ShaderProgram::Create("spot_light.program",
        {
            Shader::Create("./assets/shaders/multipass/light-volume/SpotLight.vert"),
            Shader::Create("./assets/shaders/multipass/light-volume/SpotLight.frag"),
        });
        m_SpotLightShaderProgram->Bind();
        m_SpotLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_POSITION_MAP, RenderBindingPoints::GEOMETRY_POSITION_MAP);
        m_SpotLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_NORMAL_MAP, RenderBindingPoints::GEOMETRY_NORMAL_MAP);
        m_SpotLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_ALBEDO_METALLIC_MAP, RenderBindingPoints::GEOMETRY_ALBEDO_METALLIC_MAP);
        m_SpotLightShaderProgram->SetInt(CommonShaderUniforms::GEOMETRY_EMISSIVE_ROUGHNESS_MAP, RenderBindingPoints::GEOMETRY_EMISSIVE_ROUGHNESS_MAP);
        m_SpotLightShaderProgram->SetInt(CommonShaderUniforms::SPOTLIGHT_SHADOW_MAP_ARRAY, RenderBindingPoints::SPOTLIGHT_SHADOW_MAP_ARRAY);
        m_SpotLightShaderProgram->Unbind();

        m_FrameBuffer = context.Targets.MainFrameBuffer;
    }

    void LightVolumeDeferredLightingBlinnPhongRenderPass::Execute(RenderContext& context)
    {
        ZPG_PROFILE_FUNCTION();

        // bind the main framebuffer and clear it
        m_FrameBuffer->Bind();
        RenderCommand::Clear();


        BindGeometryMaps(context);
        BindLightMaps(context);

        // copy the depth attachment from the geometry framebuffer into this framebuffer
        // this probably should be at the very end, but for debugging I need the depth to be known
        // because I render the wireframe volumes of the lights
        m_FrameBuffer->CopyAttachment(context.Targets.GeometryFrameBuffer, FrameBufferAttachmentType::Depth);


        glDisable(GL_DEPTH_TEST);


        // additive blending
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);       
        glBlendEquation(GL_FUNC_ADD);

        // render only the back face, otherwise the light doubles when both sides render
        glEnable(GL_CULL_FACE);
        glCullFace(GL_FRONT);


        // pointlight
        m_PointLightShaderProgram->Bind();
        m_PointLightShaderProgram->SetFloat2("u_ScreenSizeInv", 1.0f / v2(m_FrameBuffer->GetSpecification().Width, m_FrameBuffer->GetSpecification().Height));
        m_SphereVAO->Bind();

        int index = 0;
        for (const auto& light : context.Lights.PointLights)
        {
            glm::mat4 scale = glm::scale(glm::mat4(1.0), glm::vec3(light.Radius));
            glm::mat4 translate = glm::translate(glm::mat4(1.0), glm::vec3(light.Position));
            glm::mat4 model = translate * scale;

            m_PointLightShaderProgram->SetMat4("u_Model", model);
            m_PointLightShaderProgram->SetInt("u_Index", index++);

            RenderCommand::Draw(*m_SphereVAO);
        }

        m_SphereVAO->Unbind();
        m_PointLightShaderProgram->Unbind();


        // spotlight
        m_SpotLightShaderProgram->Bind();
        m_SpotLightShaderProgram->SetFloat2("u_ScreenSizeInv", 1.0f / v2(m_FrameBuffer->GetSpecification().Width, m_FrameBuffer->GetSpecification().Height));
        m_ConeVAO->Bind();

        // glEnable(GL_DEPTH_TEST);
        // glCullFace(GL_BACK);
        // wireframe rendering
        // glDisable(GL_CULL_FACE);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

        index = 0;
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

            m_SpotLightShaderProgram->SetMat4("u_Model", model->GetMatrix());
            m_SpotLightShaderProgram->SetInt("u_Index", index++);

            RenderCommand::Draw(*m_ConeVAO);
        }

        // restore from wireframe
        // glEnable(GL_CULL_FACE);
        // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        m_ConeVAO->Unbind();
        m_SpotLightShaderProgram->Unbind();


        // restore
        glEnable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);
        glDisable(GL_CULL_FACE);

        m_FrameBuffer->Unbind();
        RenderCommand::Clear();
    }
}
