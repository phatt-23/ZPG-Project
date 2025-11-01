#include "Renderer.h"

#include "DrawData.h"
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
#include "Skybox/Skybox.h"

namespace ZPG {

RendererAPI::API Renderer::GetAPI() {
    return RendererAPI::GetAPI();
}

void Renderer::Init() {
    ZPG_CORE_ASSERT(s_DrawData == nullptr, "Renderer already initialized.");

    RenderCommand::Init();
    s_DrawData = new DrawData();
    s_Stats = new RenderStatistics();

}

void Renderer::Shutdown() {
    ZPG_CORE_ASSERT(s_DrawData != nullptr, "Renderer isn't initialized.");

    RenderCommand::Shutdown();
    delete s_DrawData;
    delete s_Stats;
}

void Renderer::BeginDraw(const Camera& camera) {
    if (s_Deferred) {
        BeginDeferred();
    }
    else {
        s_DrawData->MainFBO->Bind();
    }

    RenderCommand::SetClearColor({0.0, 0.0, 0.0, 1.0});
    RenderCommand::Clear();

    s_DrawData->MatricesStorage.View = camera.GetViewMatrix();
    s_DrawData->MatricesStorage.Proj = camera.GetProjMatrix();
    s_DrawData->MatricesStorage.ViewProj = camera.GetViewProjMatrix();
    s_DrawData->CameraStorage.CameraPos = camera.GetPosition();

    s_DrawData->Batch.Reset();
    s_Stats->Reset();
    s_DrawData->LightsStorage.LightCount = 0;
}

void Renderer::EndDraw() {
    ZPG_CORE_ASSERT(s_DrawData);

    Flush();

    if (s_Deferred) {
        EndDeferred();
    }

    s_DrawData->MainFBO->Bind();

    if (const ref<Skybox>& skybox = s_DrawData->CurrentSkybox) {
        glDepthFunc(GL_LEQUAL);
        glDepthMask(GL_FALSE);

        const ref<VertexArray>& skyboxVAO = skybox->GetVAO();

        skybox->Bind();
        RenderCommand::DrawIndexed(*skyboxVAO, skyboxVAO->GetIndexBuffer()->GetCount());
        skybox->Unbind();

        // restore
        glDepthMask(GL_TRUE);
        glDepthFunc(GL_LESS);
    }

    s_DrawData->MainFBO->Unbind();
}

void Renderer::Flush() {
    ZPG_CORE_ASSERT(s_DrawData);

    // nothing to do
    if (s_DrawData->Batch.GetBatchSize() == 0) {
        return;
    }

    // Set up the shared SSBOs
    {
        // Matrices SSBO
        s_DrawData->MatricesSSBO.Bind();
        s_DrawData->MatricesSSBO.SetData(
            &s_DrawData->MatricesStorage,
            sizeof(DrawData::MatricesStorage));

        // Light SSBO
        s_DrawData->LightsSSBO.Bind();

        s_DrawData->LightsSSBO.SetData(
            &s_DrawData->LightsStorage.LightCount,
            sizeof(i32));

        s_DrawData->LightsSSBO.SetData(
            s_DrawData->LightsStorage.Lights,
            sizeof(LightStruct) * s_DrawData->LightsStorage.LightCount,
            offsetof(DrawData::LightsStorageBuffer, Lights));

        // Camera SSBO
        s_DrawData->CameraSSBO.Bind();
        s_DrawData->CameraSSBO.SetData(
            &s_DrawData->CameraStorage,
            sizeof(DrawData::CameraStorage));
    }

    // Batch render
    if (s_Instanced) {
        InstancedRender();
    }
    else {
        NonInstancedRender();
    }

    s_Stats->FlushCountPerFrame++;
}

void Renderer::SetLights(const std::vector<ref<Light>>& lights) {
    s_DrawData->LightsStorage.LightCount = std::min((u32)lights.size(), LightManager::s_LightCapacity);

    for (int i = 0; i < s_DrawData->LightsStorage.LightCount; i++) {
        ref<Light> light = lights[i];

        LightStruct lightStruct;

        if (light->GetLightType() == LightType::Directional) {
            lightStruct = ((DirectionalLight*)light.get())->MapToLightStruct();
        }
        else if (light->GetLightType() == LightType::Ambient) {
            lightStruct = ((AmbientLight*)light.get())->MapToLightStruct();
        }
        else if (light->GetLightType() == LightType::Point) {
            lightStruct = ((PointLight*)light.get())->MapToLightStruct();
        }
        else if (light->GetLightType() == LightType::Spotlight) {
            lightStruct = ((SpotLight*)light.get())->MapToLightStruct();
        }

        s_DrawData->LightsStorage.Lights[i] = lightStruct;
    }
}

void Renderer::SetSkybox(const ref<Skybox>& skybox) {
    s_DrawData->CurrentSkybox = skybox;
}

void Renderer::SubmitEntity(const Entity* entity, const glm::mat4& transform) {
    glm::mat4 entityTransform = transform * entity->GetTransformMatrix();
    const ref<Model>& model = entity->GetModel();
    SubmitModel(model.get(), entityTransform);
}

void Renderer::SubmitModel(const Model* model, const m4& transform) {
    const std::vector<ref<Mesh>>& meshes = model->GetMeshes();

    for (auto& mesh : meshes) {
        const ref<Material>& material = mesh->GetMaterial();
        SubmitMesh(mesh.get(), transform);
    }
}

void Renderer::SubmitMesh(const Mesh* mesh, const m4& transform) {
    // query everything out of the mesh object
    ShaderProgram* shaderProgram = mesh->GetMaterial()->GetShaderProgram().get();
    Material* material = mesh->GetMaterial().get();
    VertexArray* vertexArray = mesh->GetVertexArray().get();
    const m4& worldMat = transform * mesh->GetLocalTransform();

    if (s_DrawData->Batch.GetBatchSize() >= s_DrawData->Batch.GetBatchCapacity()) {
        Flush();
    }

    DrawCommand command(shaderProgram, material, vertexArray);
    s_DrawData->Batch.AddCommand(command, worldMat);
}

void Renderer::OnWindowResize(int width, int height) {
}

void Renderer::OnViewportResize(int width, int height) {
    s_DrawData->GBuffer->Resize(width, height);
    s_DrawData->MainFBO->Resize(width, height);
}

void Renderer::InstancedRender() {
    ZPG_CORE_ASSERT(s_DrawData);

    s_DrawData->Batch.BuildGroups();

    const auto& shaderProgramGroups = s_DrawData->Batch.GetShaderProgramGroups();
    const auto& materialGroups = s_DrawData->Batch.GetMaterialGroups();
    const auto& vaoGroups = s_DrawData->Batch.GetVertexArrayGroups();

    s_Stats->ShaderProgramGroupCount = shaderProgramGroups.size();
    s_Stats->MaterialGroupCount = materialGroups.size();
    s_Stats->VAOGroupCount = vaoGroups.size();

    // cache currently bound states to skip redundant binds
    ShaderProgram* curShaderProgram = nullptr;
    Material* curMaterial = nullptr;
    VertexArray* curVAO = nullptr;


    for (const auto& shaderProgramGroup : shaderProgramGroups) {

        ShaderProgram* shaderProgram = shaderProgramGroup.m_ShaderProgram;

        if (curShaderProgram != shaderProgram && !s_Deferred) {
            curShaderProgram = shaderProgram;

            shaderProgram->Bind();

            // these should be set only once somewhere else
            shaderProgram->SetInt(CommonShaderUniforms::ALBEDO_MAP, 0);
            shaderProgram->SetInt(CommonShaderUniforms::METALNESS_MAP, 1);
            shaderProgram->SetInt(CommonShaderUniforms::ROUGHNESS_MAP, 2);
            shaderProgram->SetInt(CommonShaderUniforms::NORMAL_MAP, 3);
            shaderProgram->SetInt(CommonShaderUniforms::EMISSIVE_MAP, 4);
        }

        for (int materialIdx = shaderProgramGroup.m_MaterialStart;
             materialIdx < shaderProgramGroup.m_MaterialStart + shaderProgramGroup.m_MaterialCount;
             materialIdx++)
        {
            const MaterialGroup& materialGroup = materialGroups[materialIdx];

            Material* material = materialGroup.m_Material;

            if (curMaterial != material) {
                curMaterial = material;

                material->GetAlbedoMap()->BindToSlot(0);
                material->GetMetalnessMap()->BindToSlot(1);
                material->GetRoughnessMap()->BindToSlot(2);
                material->GetNormalMap()->BindToSlot(3);
                material->GetEmissiveMap()->BindToSlot(4);

                s_DrawData->MaterialStorage.Albedo = material->GetAlbedo();
                s_DrawData->MaterialStorage.Emissive = material->GetEmissive();
                s_DrawData->MaterialStorage.Roughness = material->GetRoughness();
                s_DrawData->MaterialStorage.Metallic = material->GetMetallic();

                s_DrawData->MaterialSSBO.Bind();
                s_DrawData->MaterialSSBO.SetData(
                    &s_DrawData->MaterialStorage,
                    sizeof(s_DrawData->MaterialStorage));
            }

            for (int vaoIdx = materialGroup.m_VertexArrayStart;
                 vaoIdx < materialGroup.m_VertexArrayStart + materialGroup.m_VertexArrayCount;
                 vaoIdx++)
            {
                const auto& vaoGroup = vaoGroups[vaoIdx];

                VertexArray* vao = vaoGroup.m_VertexArray;

                if (curVAO != vao) {
                    curVAO = vao;

                    vao->Bind();
                }

                const m4* batchTransforms = s_DrawData->Batch.GetTransforms().data();
                memcpy(s_DrawData->ModelsStorage.Models, &batchTransforms[vaoGroup.m_Start], sizeof(m4) * vaoGroup.m_Count);

                s_DrawData->ModelsStorage.ModelCount = vaoGroup.m_Count;

                s_DrawData->ModelsSSBO.Bind();

                s_DrawData->ModelsSSBO.SetData(
                    &s_DrawData->ModelsStorage.ModelCount,
                    sizeof(i32),
                    offsetof(DrawData::ModelsStorageBuffer, ModelCount));

                s_DrawData->ModelsSSBO.SetData(
                    s_DrawData->ModelsStorage.Models,
                    sizeof(m4) * vaoGroup.m_Count,
                    offsetof(DrawData::ModelsStorageBuffer, Models));

                if (vaoGroup.m_VertexArray->HasIndexBuffer()) {
                    RenderCommand::DrawIndexedInstanced(
                        *vao,
                        vao->GetIndexBuffer()->GetCount(),
                        vaoGroup.m_Count);
                }
                else {
                    RenderCommand::DrawArraysInstanced(*vao, vaoGroup.m_Count);
                }

                s_Stats->DrawCallCountPerFrame++;
            }
        }
    }
}

void Renderer::NonInstancedRender() {
    ZPG_CORE_ASSERT(s_DrawData);

    s_DrawData->Batch.SortCommands();
    const std::vector<m4>& transforms = s_DrawData->Batch.GetTransforms();

    ShaderProgram* currentShaderProgram = nullptr;
    Material* currentMaterial = nullptr;
    VertexArray* currentVertexArray = nullptr;

    for (const auto& command : s_DrawData->Batch.GetDrawCommands()) {

        if (!currentShaderProgram || currentShaderProgram != command.m_ShaderProgram) {
            currentShaderProgram = command.m_ShaderProgram;
            // reset dependents
            currentMaterial = nullptr;
            currentVertexArray = nullptr;

            if (!s_Deferred) {
                currentShaderProgram->Bind();
                currentShaderProgram->SetInt(CommonShaderUniforms::ALBEDO_MAP, 0);
                currentShaderProgram->SetInt(CommonShaderUniforms::METALNESS_MAP, 1);
                currentShaderProgram->SetInt(CommonShaderUniforms::ROUGHNESS_MAP, 2);
                currentShaderProgram->SetInt(CommonShaderUniforms::NORMAL_MAP, 3);
                currentShaderProgram->SetInt(CommonShaderUniforms::EMISSIVE_MAP, 4);
            }
        }

        if (!currentMaterial || currentMaterial != command.m_Material) {
            currentMaterial = command.m_Material;

            // currentMaterial->Bind();  // don't, deprecated
            currentMaterial->GetAlbedoMap()->BindToSlot(0);
            currentMaterial->GetMetalnessMap()->BindToSlot(1);
            currentMaterial->GetRoughnessMap()->BindToSlot(2);
            currentMaterial->GetNormalMap()->BindToSlot(3);

            // update material SSBO per material

            s_DrawData->MaterialStorage.Albedo = currentMaterial->GetAlbedo();
            s_DrawData->MaterialStorage.Emissive = currentMaterial->GetEmissive();
            s_DrawData->MaterialStorage.Roughness = currentMaterial->GetRoughness();
            s_DrawData->MaterialStorage.Metallic = currentMaterial->GetMetallic();

            s_DrawData->MaterialSSBO.Bind();
            s_DrawData->MaterialSSBO.SetData(
                &s_DrawData->MaterialStorage,
                sizeof(s_DrawData->MaterialStorage));
        }

        if (!currentVertexArray || currentVertexArray != command.m_VAO) {
            currentVertexArray = command.m_VAO;
            currentVertexArray->Bind();
        }

        s_DrawData->ModelsStorage.Models[0] = transforms[command.m_TransformIndex];
        s_DrawData->ModelsSSBO.Bind();
        s_DrawData->ModelsSSBO.SetData(s_DrawData->ModelsStorage.Models, sizeof(m4), sizeof(v4));

        if (command.m_VAO->HasIndexBuffer()) {
            RenderCommand::DrawIndexed(*command.m_VAO, command.m_VAO->GetIndexBuffer()->GetCount());
        } else {
            RenderCommand::DrawArrays(*command.m_VAO);
        }

        s_Stats->DrawCallCountPerFrame++;
    }
}

void Renderer::BeginDeferred() {
    // bind the g-buffer so the consequent draw calls will draw into it
    s_DrawData->GBuffer->Bind();
    RenderCommand::Clear();

    s_DrawData->Batch.BuildGroups();

    // bind the gpass shader program and set the sampler slots
    s_DrawData->GPassSP->Bind();
    s_DrawData->GPassSP->SetInt(CommonShaderUniforms::ALBEDO_MAP, 0);
    s_DrawData->GPassSP->SetInt(CommonShaderUniforms::METALNESS_MAP, 1);
    s_DrawData->GPassSP->SetInt(CommonShaderUniforms::ROUGHNESS_MAP, 2);
    s_DrawData->GPassSP->SetInt(CommonShaderUniforms::NORMAL_MAP, 3);
    s_DrawData->GPassSP->SetInt(CommonShaderUniforms::EMISSIVE_MAP, 4);
}

void Renderer::EndDeferred() {

    // end of the g-buffer usage
    s_DrawData->GBuffer->Unbind();

    // bind the lighting pass shader program
    s_DrawData->LightingPassShaderProgram->Bind();

    // bind the g-buffer's color texture attachments
    // with the collected geometry information
    s_DrawData->GBuffer->BindColorTextureAttachments();

    // bind the uv quad that fills up the NDC
    s_DrawData->QuadVAO->Bind();

    // Instead of drawing the lighting pass into the default FBO, I want to draw into my own FBO.
    // Bind the main FBO (my FBO), clear it and run the lighting pass with the g-buffer's data.
    s_DrawData->MainFBO->Bind();

    RenderCommand::Clear();
    RenderCommand::DrawArrays(*s_DrawData->QuadVAO);

    // // Copy the G-Buffer's depth buffer into the main FBO's depth buffer
    s_DrawData->MainFBO->CopyAttachment(s_DrawData->GBuffer, AttachmentType::Depth);

    s_DrawData->MainFBO->Unbind();
}

}
