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
#include "Texture/Texture.h"

namespace ZPG {

DrawData* Renderer::s_DrawData = nullptr;

void Renderer::Init() {
    ZPG_CORE_ASSERT(s_DrawData == nullptr, "Renderer already initialized.");

    RenderCommand::Init();
    s_DrawData = new DrawData();
}

void Renderer::Shutdown() {
    ZPG_CORE_ASSERT(s_DrawData != nullptr, "Renderer isn't initialized.");

    RenderCommand::Shutdown();
    delete s_DrawData;
}

void Renderer::BeginDraw(const Camera& camera) {
    s_DrawData->MatricesStorage.View = camera.GetViewMatrix();
    s_DrawData->MatricesStorage.Proj = camera.GetProjMatrix();
    s_DrawData->MatricesStorage.ViewProj = camera.GetViewProjMatrix();
    s_DrawData->CameraStorage.CameraPos = camera.GetPosition();
}

void Renderer::EndDraw() {
    Renderer::Flush();
    s_DrawData->Batch.Reset();
    s_DrawData->LightsStorage.LightCount = 0;
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

    DrawCommand command{shaderProgram, material, vertexArray, worldMat};
    s_DrawData->Batch.AddCommand(command);
}

void Renderer::Flush() {
    ZPG_CORE_ASSERT(s_DrawData);

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

        LightStruct* lights = s_DrawData->LightsStorage.Lights;
        i32 lightCount = s_DrawData->LightsStorage.LightCount;

        s_DrawData->LightsSSBO.SetData(
            lights,
            lightCount * sizeof(LightStruct),
            sizeof(DrawData::LightsStorageBuffer::LightCount) + sizeof(DrawData::LightsStorageBuffer::_pad0));

        // Camera SSBO
        s_DrawData->CameraSSBO.Bind();
        s_DrawData->CameraSSBO.SetData(
            &s_DrawData->CameraStorage,
            sizeof(DrawData::CameraStorage));
    }


    // Batch render
    {
        s_DrawData->Batch.SortCommands();

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
                currentMaterial->m_AlbedoMap->BindToSlot(0);
                currentMaterial->m_MetalnessMap->BindToSlot(1);
                currentMaterial->m_RoughnessMap->BindToSlot(2);
                currentMaterial->m_NormalMap->BindToSlot(3);

                // update material SSBO per material

                s_DrawData->MaterialStorage.Albedo = currentMaterial->m_Albedo,
                s_DrawData->MaterialStorage.Emissive = currentMaterial->m_Emissive,
                s_DrawData->MaterialStorage.Roughness = currentMaterial->m_Roughness,
                s_DrawData->MaterialStorage.Metallic = currentMaterial->m_Metallic,

                s_DrawData->MaterialSSBO.Bind();
                s_DrawData->MaterialSSBO.SetData(
                    &s_DrawData->MaterialStorage,
                    sizeof(s_DrawData->MaterialStorage));
            }

            if (!currentVertexArray || currentVertexArray != command.m_VAO) {
                currentVertexArray = command.m_VAO;
                currentVertexArray->Bind();
            }

            currentShaderProgram->SetMat4(CommonShaderUniforms::MODEL, command.m_Transform);

            if (command.m_VAO->HasIndexBuffer()) {
                RenderCommand::DrawIndexed(*command.m_VAO, command.m_VAO->GetIndexBuffer()->GetCount());
            } else {
                RenderCommand::DrawArrays(*command.m_VAO);
            }
        }

        s_DrawData->Batch.Reset();
    }
}

void Renderer::OnWindowResize(int width, int height) {
    RenderCommand::SetViewport(0, 0, width, height);
}


RendererAPI::API Renderer::GetAPI() {
    return RendererAPI::GetAPI();
}

}
