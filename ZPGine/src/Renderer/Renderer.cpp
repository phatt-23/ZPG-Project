#include "Renderer.h"

#include <ranges>

#include "RenderCommand.h"
#include "Model/Model.h"
#include "Material/Material.h"
#include "Model/Mesh.h"
#include "../Camera/Camera.h"
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


struct DrawData {
    RenderBatch m_Batch = RenderBatch(1024);

    // Will be put into MatricesUBO.
    m4 m_ViewMatrix = m4(1.0);
    m4 m_ProjMatrix = m4(1.0);
    m4 m_ViewProjMatrix = m4(1.0);
    v3 m_CameraPosition = v3(0.0);


    // Will be put into LightUBO.
    LightStruct m_LightStructs[ZPG_LIGHT_UNIFORM_BUFFER_ARRAY_LENGTH];
    u32 m_LightCount = 0;


    // Uniform buffers shared by shader programs.

    struct MatricesUBO {
        m4 View;
        m4 Proj;
        m4 ViewProj;
    };

    struct LightsUBO {
        LightStruct Lights[ZPG_LIGHT_UNIFORM_BUFFER_ARRAY_LENGTH];
        i32 LightCount;
        v4 Ambient;
    };

    struct CameraUBO {
        v3 CameraPos;
    };

    struct MaterialUBO {
        v4 Albedo;
        v4 Emissive;
        f32 Roughness;
        f32 Metallic;
    };

    // model, view, proj, viewProj
    UniformBuffer m_MatricesUBO = UniformBuffer(sizeof(MatricesUBO), 0);

    // lights and light count
    UniformBuffer m_LightsUBO = UniformBuffer(sizeof(LightsUBO), 1);

    // v3 CameraPos
    UniformBuffer m_CameraUBO = UniformBuffer(sizeof(CameraUBO), 2);

    // texture maps and factors
    UniformBuffer m_MaterialUBO = UniformBuffer(sizeof(MaterialUBO), 3);
};

inline static DrawData* s_DrawData = nullptr;


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

RendererAPI::API Renderer::GetAPI() {
    return RendererAPI::GetAPI();
}

void Renderer::BeginDraw(const Camera& camera) {
    s_DrawData->m_ViewMatrix = camera.GetViewMatrix();
    s_DrawData->m_ProjMatrix = camera.GetProjMatrix();
    s_DrawData->m_ViewProjMatrix = camera.GetViewProjMatrix();
    s_DrawData->m_CameraPosition = camera.GetPosition();
}

void Renderer::EndDraw() {
    Renderer::Flush();
    s_DrawData->m_Batch.Reset();
    s_DrawData->m_LightCount = 0;
}

void Renderer::SetLights(const std::vector<ref<Light>>& lights) {
    s_DrawData->m_LightCount = std::min((u32)lights.size(), ZPG_LIGHT_UNIFORM_BUFFER_ARRAY_LENGTH);

    for (int i = 0; i < s_DrawData->m_LightCount; i++) {
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

        s_DrawData->m_LightStructs[i] = lightStruct;
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

    if (s_DrawData->m_Batch.GetBatchSize() >= s_DrawData->m_Batch.GetBatchCapacity()) {
        Flush();
    }

    DrawCommand command{shaderProgram, material, vertexArray, worldMat};
    s_DrawData->m_Batch.AddCommand(command);
}

void Renderer::Flush() {
    ZPG_CORE_ASSERT(s_DrawData);

    // Set up the UBOs
    {
        DrawData::MatricesUBO matricesUboData = {
            .View = s_DrawData->m_ViewMatrix,
            .Proj = s_DrawData->m_ProjMatrix,
            .ViewProj = s_DrawData->m_ViewProjMatrix,
        };

        DrawData::CameraUBO cameraUboData = {
            .CameraPos = s_DrawData->m_CameraPosition,
        };

        DrawData::LightsUBO lightUboData = {
            .LightCount = (i32)s_DrawData->m_LightCount,
            .Ambient = v4(1.0, 1.0, 1.0, 0.5),
        };

        memcpy(lightUboData.Lights, s_DrawData->m_LightStructs, sizeof(lightUboData.Lights));

        // global
        // binding slot 0
        s_DrawData->m_MatricesUBO.Bind();
        s_DrawData->m_MatricesUBO.SetData(&matricesUboData, sizeof(matricesUboData));

        // binding slot 1
        s_DrawData->m_LightsUBO.Bind();
        s_DrawData->m_LightsUBO.SetData(&lightUboData, sizeof(lightUboData));

        // binding slot 2
        s_DrawData->m_CameraUBO.Bind();
        s_DrawData->m_CameraUBO.SetData(&cameraUboData, sizeof(cameraUboData));
    }

    ShaderProgram* currentShaderProgram = nullptr;
    Material* currentMaterial = nullptr;
    VertexArray* currentVertexArray = nullptr;

    s_DrawData->m_Batch.SortCommands();

    for (const auto& command : s_DrawData->m_Batch.GetDrawCommands()) {

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

            // currentMaterial->Bind();
            currentMaterial->m_AlbedoMap->BindToSlot(0);
            currentMaterial->m_MetalnessMap->BindToSlot(1);
            currentMaterial->m_RoughnessMap->BindToSlot(2);
            currentMaterial->m_NormalMap->BindToSlot(3);

            // update material UBO per material
            DrawData::MaterialUBO materialUboData = {
                .Albedo = currentMaterial->m_Albedo,
                .Emissive = currentMaterial->m_Emissive,
                .Roughness = currentMaterial->m_Roughness,
                .Metallic = currentMaterial->m_Metallic,
            };

            s_DrawData->m_MaterialUBO.Bind();
            s_DrawData->m_MaterialUBO.SetData(&materialUboData, sizeof(materialUboData));
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

    s_DrawData->m_Batch.Reset();
}

void Renderer::OnWindowResize(int width, int height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

}
