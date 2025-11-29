//
// Created by phatt on 11/16/25.
//

#include "DeferredLightingBlinnPhongRenderPass.h"

#include "Buffer/BufferLayout.h"
#include "Buffer/FrameBuffer.h"
#include "Buffer/VertexArray.h"
#include "Material/Material.h"
#include "Model/Mesh.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/RenderContext.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/Shader.h"
#include "Shader/ShaderDataType.h"
#include "Shader/ShaderProgram.h"
#include "Texture/Texture2D.h"
#include "Texture/Texture2DArray.h"
#include "Texture/TextureCubeMapArray.h"

namespace ZPG
{

    void DeferredLightingBlinnPhongRenderPass::Init(RenderContext& context)
    {
        m_ShaderProgram = ShaderProgram::Create("deferred_blinn_phong.program",
        {
            Shader::Create("./assets/shaders/multipass/Deferred-BlinnPhong.vert"),
            Shader::Create("./assets/shaders/multipass/Deferred-BlinnPhong.frag"),
        });

        m_ShaderProgram->Bind();

        m_ShaderProgram->SetInt("g_Color0", RenderBindingPoints::GEOMETRY_POSITION_MAP);
        m_ShaderProgram->SetInt("g_Color1", RenderBindingPoints::GEOMETRY_NORMAL_MAP);
        m_ShaderProgram->SetInt("g_Color2", RenderBindingPoints::GEOMETRY_ALBEDO_METALLIC_MAP);
        m_ShaderProgram->SetInt("g_Color3", RenderBindingPoints::GEOMETRY_EMISSIVE_ROUGHNESS_MAP);
        m_ShaderProgram->SetInt("g_Color4", RenderBindingPoints::GEOMETRY_ENTITY_ID_MAP);

        m_ShaderProgram->SetInt(CommonShaderUniforms::DIRECTIONAL_LIGHT_SHADOW_MAP_ARRAY, RenderBindingPoints::DIRECTIONAL_LIGHT_SHADOW_MAP_ARRAY);
        m_ShaderProgram->SetInt(CommonShaderUniforms::SPOTLIGHT_SHADOW_MAP_ARRAY, RenderBindingPoints::SPOTLIGHT_SHADOW_MAP_ARRAY);
        m_ShaderProgram->SetInt(CommonShaderUniforms::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY, RenderBindingPoints::POINTLIGHT_SHADOW_CUBE_MAP_ARRAY);
        m_ShaderProgram->SetInt(CommonShaderUniforms::SKYBOX_CUBEMAP, RenderBindingPoints::SKYBOX_TEXTURE_SLOT);
        m_ShaderProgram->SetInt(CommonShaderUniforms::SKYDOME_MAP, RenderBindingPoints::SKYDOME_TEXTURE_SLOT);

        m_ShaderProgram->Unbind();

        m_FrameBuffer = context.Targets.MainFrameBuffer;
    }

    void DeferredLightingBlinnPhongRenderPass::Execute(RenderContext& context)
    {
        m_FrameBuffer->Bind();
        RenderCommand::Clear();
        m_ShaderProgram->Bind();
        m_ScreenQuadVAO->Bind();

        BindGeometryMaps(context);
        BindLightMaps(context);

        RenderCommand::Draw(*m_ScreenQuadVAO);

        m_ScreenQuadVAO->Unbind();
        m_ShaderProgram->Unbind();
        m_FrameBuffer->Unbind();
        m_FrameBuffer->CopyAttachment(context.Targets.GeometryFrameBuffer, FrameBufferAttachmentType::Depth);
    }
}
