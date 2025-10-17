#include "Renderer.h"
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
#include "Scene/Scene.h"

namespace ZPG {

void Renderer::Init() {
    RenderCommand::Init();
    s_DrawData = MakeScope<DrawData>();
}

void Renderer::Shutdown() {
    RenderCommand::Shutdown();
}

void Renderer::BeginDraw(const Camera& camera) {
    s_DrawData->ViewProjMatrix = camera.GetViewProjMatrix();
    s_DrawData->CameraPosition = camera.GetPosition();
}

void Renderer::EndDraw() {
    // TODO:
    // nothing for now
    // batch the drawing by instances of the same DrawableObject, shader being used and material
    // setup the submitted lights into the shaders
    // draw as little as possible


    s_DrawData->Lights.clear();
}

void Renderer::Submit(ShaderProgram& shaderProgram, const Entity& entity, const glm::mat4& transform) {
    glm::mat4 mat = transform * entity.GetTransformMatrix();
    const ref<Model>& model = entity.GetModel();

    Submit(shaderProgram, *model, mat);
}

void Renderer::SubmitEntity(const Entity& entity, const glm::mat4& transform) {
    glm::mat4 entityTransform = transform * entity.GetTransformMatrix();
    const ref<Model>& model = entity.GetModel();
    std::vector<ref<Mesh>> meshes = model->GetMeshes();

    for (auto& mesh : meshes) {
        const ref<Material>& material = mesh->GetMaterial();
        SubmitMesh(*mesh, entityTransform);
    }
}

void Renderer::SubmitMesh(const Mesh& mesh, const glm::mat4& transform) {
    glm::mat4 worldMat = transform * mesh.GetLocalTransform();
    const ref<Material>& material = mesh.GetMaterial();

    if (material) {
        material->Bind();
    } else {
        ZPG_CORE_WARN("No shader program attached to a material.");
    }

    Submit(*material->GetShaderProgram(), *mesh.GetVertexArray(), worldMat);
}


void Renderer::Submit(ShaderProgram& shaderProgram, const Model& model, const glm::mat4& transform) {
    const std::vector<ref<Mesh>>& meshes = model.GetMeshes();

    for (const ref<Mesh>& mesh : meshes) {
        Submit(shaderProgram, *mesh, transform);
    }
}

void Renderer::Submit(ShaderProgram& shaderProgram, const Mesh& mesh, const glm::mat4& transform) {
    glm::mat4 worldMat = transform * mesh.GetLocalTransform();
    const ref<Material>& material = mesh.GetMaterial();

    if (material) {
        material->Bind();
    } else {
        ZPG_CORE_WARN("No shader program attached to a material.");
    }

    // without dereference this method calls itself, stack overflow 
    Submit(shaderProgram, *mesh.GetVertexArray(), worldMat);
}

void Renderer::RenderScene(const Scene& scene) {
    ZPG_NOT_IMPL();
}

void Renderer::Submit(ShaderProgram& shaderProgram, const VertexArray& vertexArray, const glm::mat4& transform) {
    shaderProgram.Bind();
    shaderProgram.SetMat4("u_Model", transform);
    shaderProgram.SetMat4("u_ViewProj", s_DrawData->ViewProjMatrix);
    shaderProgram.SetFloat3("u_CameraPos", s_DrawData->CameraPosition);

    // TODO: optimize
    auto& lights = s_DrawData->Lights;
    size_t index = 0;
    for (index = 0; index < lights.size(); index++) {
        if (lights[index] == nullptr) 
            continue;
        lights[index]->SendToShaderProgram(shaderProgram, index);
    }
    shaderProgram.SetInt("u_LightCount", (i32)index);

    vertexArray.Bind();
    if (vertexArray.HasIndexBuffer()) {
        RenderCommand::DrawIndexed(vertexArray);
    } else {
        RenderCommand::DrawArrays(vertexArray);
    }
}

void Renderer::SetLights(const std::vector<ref<Light>>& lights) {
    s_DrawData->Lights = lights;

}

void Renderer::OnWindowResize(int width, int height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

}
