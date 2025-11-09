#include "Renderer.h"

#include "DrawData.h"
#include "Renderer/RenderBindingPoints.h"
#include "imgui_internal.h"
#include "RenderCommand.h"
#include "Model/Model.h"
#include "Material/Material.h"
#include "Model/Mesh.h"
#include "../Camera/Camera.h"
#include "Buffer/ShaderStorageBuffer.h"
#include "Buffer/VertexArray.h"
#include "Debug/Asserter.h"
#include "Debug/Logger.h"
#include "Shader/ShaderProgram.h"
#include "Light/Light.h"
#include "Entity/Entity.h"
#include "Light/AmbientLight.h"
#include "Light/DirectionalLight.h"
#include "Light/PointLight.h"
#include "Light/SpotLight.h"
#include "Scene/Scene.h"
#include "Shader/CommonShaderUniforms.h"
#include "RenderGroups.h"
#include "Texture/Texture.h"
#include "RenderStatistics.h"
#include "Profiling/Instrumentor.h"
#include "Resource/CommonResources.h"
#include "Sky/Skybox.h"
#include "Sky/Skydome.h"

namespace ZPG {

RendererAPI::API Renderer::GetAPI() 
{
    ZPG_PROFILE_FUNCTION();
    return RendererAPI::GetAPI();
}

void Renderer::Init() 
{
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(s_Draw == nullptr, "Renderer already initialized.");

    // RenderCommand::Init();
    s_Draw = new DrawData();
    s_Stats = new RenderStatistics();
}

void Renderer::Shutdown() 
{
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(s_Draw != nullptr, "Renderer isn't initialized.");

    // RenderCommand::Shutdown();
    delete s_Draw;
    delete s_Stats;
}

void Renderer::BeginDraw(const Camera& camera) 
{
    ZPG_PROFILE_FUNCTION();
    // bind the g-buffer so the consequent draw calls will draw into it
    s_Draw->GBuffer->Bind();
    RenderCommand::Clear();

    // bind the gpass shader program and set the sampler slots
    s_Draw->GPassSP->Bind();
    s_Draw->GPassSP->SetInt(CommonShaderUniforms::ALBEDO_MAP,       RenderBindingPoints::ALBEDO_MAP);
    s_Draw->GPassSP->SetInt(CommonShaderUniforms::METALNESS_MAP,    RenderBindingPoints::METALNESS_MAP);
    s_Draw->GPassSP->SetInt(CommonShaderUniforms::ROUGHNESS_MAP,    RenderBindingPoints::ROUGHNESS_MAP);
    s_Draw->GPassSP->SetInt(CommonShaderUniforms::NORMAL_MAP,       RenderBindingPoints::NORMAL_MAP);
    s_Draw->GPassSP->SetInt(CommonShaderUniforms::EMISSIVE_MAP,     RenderBindingPoints::EMISSIVE_MAP);

    s_Draw->MatricesStorage.View = camera.GetViewMatrix();
    s_Draw->MatricesStorage.Proj = camera.GetProjMatrix();
    s_Draw->MatricesStorage.ViewProj = camera.GetViewProjMatrix();
    s_Draw->CameraStorage.CameraPos = camera.GetPosition();

    s_Draw->Batch.Reset();
    s_Stats->Reset();
    s_Draw->LightsStorage.LightCount = 0;
}

void Renderer::EndDraw() 
{
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(s_Draw);
    Flush();

    // end of the g-buffer usage
    s_Draw->GBuffer->Unbind();

    // Instead of drawing the lighting pass into the default FBO, I want to draw into my own FBO.
    // Bind the main FBO (my FBO), clear it and run the lighting pass with the g-buffer's data.
    s_Draw->MainFBO->Bind();
    RenderCommand::Clear();

    // bind the lighting pass shader program
    s_Draw->LightingPassShaderProgram->Bind();


    // bind dummy skybox map texture
    ResourceManager::GetGlobal().GetTexture(CommonResources::NULL_CUBEMAP)->BindToSlot(RenderBindingPoints::SKYBOX_TEXTURE_SLOT);
    s_Draw->LightingPassShaderProgram->SetInt(CommonShaderUniforms::SKYBOX_CUBEMAP, RenderBindingPoints::SKYBOX_TEXTURE_SLOT);

    // bind dummy skydome map texture
    ResourceManager::GetGlobal().GetTexture(CommonResources::NULL_SKYDOME_MAP)->BindToSlot(RenderBindingPoints::SKYDOME_TEXTURE_SLOT);
    s_Draw->LightingPassShaderProgram->SetInt(CommonShaderUniforms::SKYDOME_MAP, RenderBindingPoints::SKYDOME_TEXTURE_SLOT);


    switch (s_Draw->CurrentSky->GetSkyType()) 
    {
        case SkyType::Skybox:
            s_Draw->LightingPassShaderProgram->SetInt(CommonShaderUniforms::SKYTYPE, (i32)SkyType::Skybox);
            s_Draw->CurrentSky->BindTextureToSlot(RenderBindingPoints::SKYBOX_TEXTURE_SLOT); // this must come first
            s_Draw->LightingPassShaderProgram->SetInt(CommonShaderUniforms::SKYBOX_CUBEMAP, RenderBindingPoints::SKYBOX_TEXTURE_SLOT);
            break;

        case SkyType::Skydome: 
            s_Draw->LightingPassShaderProgram->SetInt(CommonShaderUniforms::SKYTYPE, (i32)SkyType::Skydome);
            s_Draw->CurrentSky->BindTextureToSlot(RenderBindingPoints::SKYDOME_TEXTURE_SLOT); // this must come first
            s_Draw->LightingPassShaderProgram->SetInt(CommonShaderUniforms::SKYDOME_MAP, RenderBindingPoints::SKYDOME_TEXTURE_SLOT);
            break;

        case SkyType::None:
            ZPG_UNREACHABLE("SkyType::None not supported")
    }

    // bind the g-buffer's color texture attachments
    // with the collected geometry information
    s_Draw->GBuffer->BindColorTextureAttachments();

    s_Draw->QuadVAO->Bind(); // bind the uv quad that fills up the NDC
    RenderCommand::DrawArrays(*s_Draw->QuadVAO);
    s_Draw->QuadVAO->Unbind();

    // bind the lighting pass shader program
    s_Draw->LightingPassShaderProgram->Unbind();

    // // Copy the G-Buffer's depth buffer into the main FBO's depth buffer
    s_Draw->MainFBO->CopyAttachment(s_Draw->GBuffer, FrameBufferAttachmentType::Depth);

    s_Draw->MainFBO->Unbind();

    s_Draw->MainFBO->Bind();

    if (s_Draw->CurrentSky) 
    {
        const ref<VertexArray>& skyVertexArray = s_Draw->CurrentSky->GetVertexArray();

        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        s_Draw->CurrentSky->Bind();
        RenderCommand::DrawIndexed(*skyVertexArray, skyVertexArray->GetIndexBuffer()->GetCount());
        s_Draw->CurrentSky->Unbind();
        
        glDepthMask(GL_TRUE); // restore
        glDepthFunc(GL_LESS);
    }

    s_Draw->MainFBO->Unbind();
}

void Renderer::Flush() {
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(s_Draw);

    // nothing to do
    if (s_Draw->Batch.GetBatchSize() == 0) {
        return;
    }

    // Set up the shared SSBOs
    {
        // Matrices SSBO
        s_Draw->MatricesSSBO.Bind();
        s_Draw->MatricesSSBO.SetData(
            &s_Draw->MatricesStorage,
            sizeof(DrawData::MatricesStorage));

        // Light SSBO
        s_Draw->LightsSSBO.Bind();

        s_Draw->LightsSSBO.SetData(
            &s_Draw->LightsStorage.LightCount,
            sizeof(i32));

        s_Draw->LightsSSBO.SetData(
            s_Draw->LightsStorage.Lights,
            sizeof(LightStruct) * s_Draw->LightsStorage.LightCount,
            offsetof(DrawData::LightsStorageBuffer, Lights));

        // Camera SSBO
        s_Draw->CameraSSBO.Bind();
        s_Draw->CameraSSBO.SetData(
            &s_Draw->CameraStorage,
            sizeof(DrawData::CameraStorage));
    }

    // Batch render
    InstancedRender();

    s_Stats->FlushCountPerFrame++;
}

void Renderer::SetLights(const std::vector<ref<Light>>& lights) 
{
    ZPG_PROFILE_FUNCTION();
    s_Draw->LightsStorage.LightCount = std::min((u32)lights.size(), LightManager::s_LightCapacity);

    for (int i = 0; i < s_Draw->LightsStorage.LightCount; i++) {
        ref<Light> light = lights[i];

        LightStruct lightStruct;

        switch (light->GetLightType()) 
        {
            case LightType::None: 
                ZPG_UNREACHABLE("LightType::None not supported"); 
                break;
            case LightType::Directional:
                lightStruct = ((DirectionalLight*)light.get())->MapToLightStruct();
                break;
            case LightType::Ambient:
                lightStruct = ((AmbientLight*)light.get())->MapToLightStruct();
                break;
            case LightType::Point:
                lightStruct = ((PointLight*)light.get())->MapToLightStruct();
                break;
            case LightType::Spotlight:
                lightStruct = ((SpotLight*)light.get())->MapToLightStruct();
                break;
        }

        s_Draw->LightsStorage.Lights[i] = lightStruct;
    }
}

void Renderer::SetSkybox(const ref<Skybox>& skybox) 
{
    ZPG_PROFILE_FUNCTION();
    s_Draw->CurrentSky = skybox;
}

void Renderer::SetSkydome(const ref<Skydome>& skydome) 
{
    ZPG_PROFILE_FUNCTION();
    s_Draw->CurrentSky = skydome;
}

void Renderer::SubmitEntity(const Entity* entity, const glm::mat4& transform) 
{
    ZPG_PROFILE_FUNCTION();
    const glm::mat4& entityTransform = transform * entity->GetTransformMatrix();
    const ref<Model>& model = entity->GetModel();
    SubmitModel(model.get(), entityTransform, entity->GetEntityID());
}

void Renderer::SubmitModel(const Model* model, const m4& transform, int entityID) 
{
    ZPG_PROFILE_FUNCTION();
    const std::vector<ref<Mesh>>& meshes = model->GetMeshes();

    for (auto& mesh : meshes) 
    {
        SubmitMesh(mesh.get(), transform, entityID);
    }
}

void Renderer::SubmitMesh(const Mesh* mesh, const m4& transform, int entityID) 
{
    ZPG_PROFILE_FUNCTION();
    // query everything out of the mesh object
    ShaderProgram* shaderProgram = mesh->GetMaterial()->GetShaderProgram().get();
    Material* material = mesh->GetMaterial().get();
    VertexArray* vertexArray = mesh->GetVertexArray().get();
    const m4& worldMat = transform * mesh->GetLocalTransform();

    if (s_Draw->Batch.GetBatchSize() >= s_Draw->Batch.GetBatchCapacity()) 
    {
        Flush();
    }

    DrawCommand command(shaderProgram, material, vertexArray);
    s_Draw->Batch.AddCommand(command, worldMat, entityID);
}

void Renderer::OnViewportResize(int width, int height) 
{
    ZPG_PROFILE_FUNCTION();
    s_Draw->GBuffer->Resize(width, height);
    s_Draw->MainFBO->Resize(width, height);
}

void Renderer::InstancedRender() 
{
    ZPG_PROFILE_FUNCTION();
    ZPG_CORE_ASSERT(s_Draw);

    s_Draw->Batch.BuildGroups();

    const auto& shaderProgramGroups = s_Draw->Batch.GetShaderProgramGroups();
    const auto& materialGroups = s_Draw->Batch.GetMaterialGroups();
    const auto& vaoGroups = s_Draw->Batch.GetVertexArrayGroups();

    s_Stats->ShaderProgramGroupCount = shaderProgramGroups.size();
    s_Stats->MaterialGroupCount = materialGroups.size();
    s_Stats->VAOGroupCount = vaoGroups.size();

    // cache currently bound states to skip redundant binds
    ShaderProgram* curShaderProgram = nullptr;
    Material* curMaterial = nullptr;
    VertexArray* curVAO = nullptr;


    for (const auto& shaderProgramGroup : shaderProgramGroups) 
    {

        ShaderProgram* shaderProgram = shaderProgramGroup.m_ShaderProgram;

        /*
        if (curShaderProgram != shaderProgram && !s_Deferred) 
        {
            curShaderProgram = shaderProgram;

            shaderProgram->Bind();

            // these should be set only once somewhere else
            shaderProgram->SetInt(CommonShaderUniforms::ALBEDO_MAP, 0);
            shaderProgram->SetInt(CommonShaderUniforms::METALNESS_MAP, 1);
            shaderProgram->SetInt(CommonShaderUniforms::ROUGHNESS_MAP, 2);
            shaderProgram->SetInt(CommonShaderUniforms::NORMAL_MAP, 3);
            shaderProgram->SetInt(CommonShaderUniforms::EMISSIVE_MAP, 4);
        }
         */

        for (int materialIdx = shaderProgramGroup.m_MaterialStart;
             materialIdx < shaderProgramGroup.m_MaterialStart + shaderProgramGroup.m_MaterialCount;
             materialIdx++)
        {
            const MaterialGroup& materialGroup = materialGroups[materialIdx];

            Material* material = materialGroup.m_Material;

            if (curMaterial != material) 
            {
                curMaterial = material;

                material->GetAlbedoMap()->BindToSlot(0);
                material->GetMetalnessMap()->BindToSlot(1);
                material->GetRoughnessMap()->BindToSlot(2);
                material->GetNormalMap()->BindToSlot(3);
                material->GetEmissiveMap()->BindToSlot(4);

                s_Draw->MaterialStorage.Albedo = material->GetAlbedo();
                s_Draw->MaterialStorage.Emissive = material->GetEmissive();
                s_Draw->MaterialStorage.Roughness = material->GetRoughness();
                s_Draw->MaterialStorage.Metallic = material->GetMetallic();

                s_Draw->MaterialSSBO.Bind();
                s_Draw->MaterialSSBO.SetData(
                    &s_Draw->MaterialStorage,
                    sizeof(s_Draw->MaterialStorage));
            }

            for (int vaoIdx = materialGroup.m_VertexArrayStart;
                 vaoIdx < materialGroup.m_VertexArrayStart + materialGroup.m_VertexArrayCount;
                 vaoIdx++)
            {
                const auto& vaoGroup = vaoGroups[vaoIdx];

                VertexArray* vao = vaoGroup.m_VertexArray;

                if (curVAO != vao) 
                {
                    curVAO = vao;
                    vao->Bind();
                }

                // ModelsSSBO
                {
                    const m4* batchTransforms = s_Draw->Batch.GetTransforms().data();
                    memcpy(s_Draw->ModelsStorage.Models, &batchTransforms[vaoGroup.m_Start], sizeof(m4) * vaoGroup.m_Count);

                    s_Draw->ModelsStorage.ModelCount = vaoGroup.m_Count;

                    s_Draw->ModelsSSBO.Bind();

                    s_Draw->ModelsSSBO.SetData(
                        &s_Draw->ModelsStorage.ModelCount,
                        sizeof(i32),
                        offsetof(DrawData::ModelsStorageBuffer, ModelCount));

                    s_Draw->ModelsSSBO.SetData(
                        s_Draw->ModelsStorage.Models,
                        sizeof(m4) * vaoGroup.m_Count,
                        offsetof(DrawData::ModelsStorageBuffer, Models));
                }

                // EntitiesSSBO
                {
                    const i32* entityIDs = s_Draw->Batch.GetEntityIDs().data();

                    memcpy(s_Draw->EntitiesStorage.EntityIDs, &entityIDs[vaoGroup.m_Start], sizeof(i32) * vaoGroup.m_Count);

                    s_Draw->EntitiesStorage.EntityCount = vaoGroup.m_Count;

                    s_Draw->EntitiesSSBO.Bind();

                    s_Draw->EntitiesSSBO.SetData(
                        &s_Draw->EntitiesStorage.EntityCount,
                        sizeof(i32),
                        offsetof(DrawData::EntitiesStorageBuffer, EntityCount));

                    s_Draw->EntitiesSSBO.SetData(
                        s_Draw->EntitiesStorage.EntityIDs,
                        sizeof(i32) * vaoGroup.m_Count,
                        offsetof(DrawData::EntitiesStorageBuffer, EntityIDs));
                }

                if (vaoGroup.m_VertexArray->HasIndexBuffer()) 
                {
                    RenderCommand::DrawIndexedInstanced(
                        *vao,
                        vao->GetIndexBuffer()->GetCount(),
                        vaoGroup.m_Count);
                }
                else 
                {
                    RenderCommand::DrawArraysInstanced(*vao, vaoGroup.m_Count);
                }

                s_Stats->DrawCallCountPerFrame++;
            }
        }
    }
}


}
