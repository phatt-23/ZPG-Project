#include "Renderer.h"

#include "DrawData.h"
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

namespace ZPG {

void Renderer::Init() {
    ZPG_CORE_ASSERT(s_DrawData == nullptr, "Renderer already initialized.");

    RenderCommand::Init();
    s_DrawData = new DrawData();
    s_Stats = new Statistics();
}

void Renderer::Shutdown() {
    ZPG_CORE_ASSERT(s_DrawData != nullptr, "Renderer isn't initialized.");

    RenderCommand::Shutdown();
    delete s_DrawData;
    delete s_Stats;
}

void Renderer::BeginDraw(const Camera& camera) {
    s_DrawData->MatricesStorage.View = camera.GetViewMatrix();
    s_DrawData->MatricesStorage.Proj = camera.GetProjMatrix();
    s_DrawData->MatricesStorage.ViewProj = camera.GetViewProjMatrix();
    s_DrawData->CameraStorage.CameraPos = camera.GetPosition();

    s_DrawData->Batch.Reset();
    s_Stats->Reset();
    s_DrawData->LightsStorage.LightCount = 0;
}

void Renderer::EndDraw() {
    Flush();
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

void Renderer::RenderScene(const Scene& scene) {
    ZPG_NOT_IMPL();
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

void Renderer::SubmitMesh(const Mesh* mesh, const glm::mat4& transform) {
    // query everything out of the mesh object
    ShaderProgram*  shaderProgram   = mesh->GetMaterial()->GetShaderProgram().get();
    Material*       material        = mesh->GetMaterial().get();
    VertexArray*    vertexArray     = mesh->GetVertexArray().get();
    m4              worldMat        = transform * mesh->GetLocalTransform();

    if (s_DrawData->Batch.GetBatchSize() >= s_DrawData->Batch.GetBatchCapacity()) {
        Flush();
    }

    DrawCommand command(shaderProgram, material, vertexArray);
    s_DrawData->Batch.AddCommand(command, worldMat);
}

void Renderer::Flush() {
    ZPG_CORE_ASSERT(s_DrawData);

    // nothing to do
    if (s_DrawData->Batch.GetBatchSize() == 0) {
        return;
    }

    // Set up the SSBOs
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
    if (s_Instanced) // instanced way
    {
        s_DrawData->Batch.BuildGroups();

        const auto& shaderProgramGroups = s_DrawData->Batch.GetShaderProgramGroups();
        const auto& materialGroups = s_DrawData->Batch.GetMaterialGroups();
        const auto& vaoGroups = s_DrawData->Batch.GetVertexArrayGroups();

        ZPG_CORE_INFO("shader groups: {}, material groups: {}, vao groups: {}", 
                      shaderProgramGroups.size(), materialGroups.size(), vaoGroups.size());

        const auto& drawCommands = s_DrawData->Batch.GetDrawCommands();

        // cache current bound states to skip redundant binds
        ShaderProgram* curShaderProgram = nullptr;
        Material* curMaterial = nullptr;
        VertexArray* curVAO = nullptr;

        // handle to models storage buffer's models array
        m4* modelsDest = s_DrawData->ModelsStorage.Models;

        const m4* batchTransforms = s_DrawData->Batch.GetTransforms().data();

        for (const auto& shaderProgramGroup : shaderProgramGroups) {

            ShaderProgram* shaderProgram = shaderProgramGroup.m_ShaderProgram;

            if (curShaderProgram != shaderProgram) {
                curShaderProgram = shaderProgram;

                shaderProgram->Bind();

                // these should be set only once somewhere else
                shaderProgram->SetInt(CommonShaderUniforms::ALBEDO_MAP, 0);
                shaderProgram->SetInt(CommonShaderUniforms::METALNESS_MAP, 1);
                shaderProgram->SetInt(CommonShaderUniforms::ROUGHNESS_MAP, 2);
                shaderProgram->SetInt(CommonShaderUniforms::NORMAL_MAP, 3);
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
                   
            #if 0
                    void* modelsSSBOMap = s_DrawData->ModelsSSBO.Map();

                    i32* modelCountPtr = (i32*)(modelsSSBOMap);
                    m4* modelsMapPtr = (m4*)((char*)modelsSSBOMap + offsetof(DrawData::ModelsStorageBuffer, Models));

                    *modelCountPtr = (i32)vaoGroup.m_Count;
                    memcpy( modelsMapPtr, &batchTransforms[vaoGroup.m_Start], sizeof(m4) * vaoGroup.m_Count );

                    s_DrawData->ModelsSSBO.Unmap();

            #else

                    memcpy( s_DrawData->ModelsStorage.Models, &batchTransforms[vaoGroup.m_Start], sizeof(m4) * vaoGroup.m_Count );
                    
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
            #endif

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

        
        s_DrawData->Batch.Reset();
    }
    else // non instanced way
    {
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

                currentShaderProgram->Bind();
                currentShaderProgram->SetInt(CommonShaderUniforms::ALBEDO_MAP, 0);
                currentShaderProgram->SetInt(CommonShaderUniforms::METALNESS_MAP, 1);
                currentShaderProgram->SetInt(CommonShaderUniforms::ROUGHNESS_MAP, 2);
                currentShaderProgram->SetInt(CommonShaderUniforms::NORMAL_MAP, 3);
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

        s_DrawData->Batch.Reset();
    }

    s_Stats->FlushCountPerFrame++;
}

void Renderer::OnWindowResize(int width, int height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

RendererAPI::API Renderer::GetAPI() {
    return RendererAPI::GetAPI();
}

}
