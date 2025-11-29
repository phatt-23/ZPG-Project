#include "LightingRenderPass.h"
#include "Debug/Asserter.h"
#include "Renderer/RenderBindingPoints.h"
#include "Renderer/RenderContext.h"
#include "Renderer/RenderCommand.h"
#include "Resource/ResourceManager.h"
#include "Shader/ShaderProgram.h"
#include "Texture/TextureCubeMap.h"
#include "Shader/CommonShaderUniforms.h"
#include "Shader/Shader.h"
#include "Buffer/BufferLayout.h"
#include "Buffer/VertexArray.h"
#include "Buffer/VertexBuffer.h"
#include "Profiling/Instrumentor.h"
#include "Texture/Texture2DArray.h"
#include "Texture/TextureCubeMapArray.h"

namespace ZPG
{
    LightingRenderPass::LightingRenderPass()
    {
        ZPG_PROFILE_FUNCTION();

        m_ShaderProgram = ShaderProgram::Create("LightingPass",
        {
            Shader::Create("assets/shaders/multipass/LightingPass.vert"),
            Shader::Create("assets/shaders/multipass/LightingPass.frag"),
        });
    }

    void LightingRenderPass::Init(RenderContext &context)
    {
        ZPG_PROFILE_FUNCTION();

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

    void LightingRenderPass::Execute(RenderContext& context)
    {
        ZPG_PROFILE_FUNCTION();

        m_FrameBuffer->Bind();

        RenderCommand::Clear();
        m_ShaderProgram->Bind(); 

        BindSky(context);
        BindGeometryMaps(context); 
        BindLightMaps(context);

        m_ScreenQuadVAO->Bind(); // bind the uv quad that fills up the NDC
        RenderCommand::Draw(*m_ScreenQuadVAO);
        m_ScreenQuadVAO->Unbind();

        m_ShaderProgram->Unbind();
        m_FrameBuffer->Unbind();
    }
}

