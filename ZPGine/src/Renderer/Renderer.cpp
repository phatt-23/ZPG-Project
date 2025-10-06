#include "Renderer.h"
#include "RenderCommand.h"

namespace ZPG {

void Renderer::Init() {
    RenderCommand::Init();
    s_DrawData = CreateScope<DrawData>();
    s_ShaderProgramLibrary = CreateScope<ShaderProgramLibrary>();
}

void Renderer::Shutdown() {
    RenderCommand::Shutdown();
}

void Renderer::BeginDraw(const Camera& camera) {
    s_DrawData->ViewProjMatrix = camera.GetViewProjMatrix();
    s_DrawData->CameraPosition = camera.GetPosition();
}

void Renderer::EndDraw() {
    // nothing for now
    // batch the drawing by instances of the same DrawableObject, shader being used and material
    // setup the submitted lights into the shaders
    s_DrawData->Lights.clear();
}

void Renderer::Submit(ShaderProgram& shaderProgram, const Entity& entity, const glm::mat4& transform) {
    glm::mat4 mat = transform * entity.GetTransformMatrix();
    const Ref<Model>& model = entity.GetModel();

    Submit(shaderProgram, *model, mat);
}

void Renderer::Submit(ShaderProgram& shaderProgram, const Model& model, const glm::mat4& transform) {
    const std::vector<Ref<Mesh>>& meshes = model.GetMeshes();

    for (Ref<Mesh> mesh : meshes) {
        Submit(shaderProgram, *mesh, transform);
    }
}

void Renderer::Submit(ShaderProgram& shaderProgram, const Mesh& mesh, const glm::mat4& transform) {
    glm::mat4 worldMat = transform * mesh.GetLocalTransform();

    // without dereference this method calls itself, stack overflow 
    Submit(shaderProgram, *mesh.GetVertexArray(), worldMat);
}

// Použití observer patternu mezi ShaderProgramem a Camerou se v mém návrhu nehodí.
// 
// Mohl bych mít 10 shader programů, z toho je aktivních třeba pouze 1.
// Potom by se event callback funkcí zavolalo 10 při každé změně pozice kamery.
// To však není nutné a bude to žrát výkon + kód bude méně přehledný z nutnosti provazat 
// tyto dvě třídy, které by o sobě nejlépe neměli ani vědět.
// 
// Vykreslování v enginu má na starosti třída Renderer. 
// Té se předá na začátku vykreslování reference na Cameru.
// Potom se submitují objekty, které chci vykreslit a Renderer se s tím vypořádá.
// 
// Takhle se stará o vykreslování pouze jeden objekt - třída Renderer - a žádná jiná
// nemusí implementovat .draw() metodu.
void Renderer::Submit(ShaderProgram& shaderProgram, const VertexArray& vertexArray, const glm::mat4& transform) {
    shaderProgram.Bind();
    shaderProgram.SetMat4("u_Model", transform);
    shaderProgram.SetMat4("u_ViewProj", s_DrawData->ViewProjMatrix);
    shaderProgram.SetFloat3("u_CameraPos", s_DrawData->CameraPosition);

    auto& lights = s_DrawData->Lights;
    size_t index = 0;
    for (index = 0; index < lights.size(); index++) {
        if (lights[index] == nullptr) 
            continue;
        lights[index]->SendToShaderProgram(shaderProgram, index);
    }
    shaderProgram.SetInt("u_LightCount", index);

    vertexArray.Bind();
    if (vertexArray.HasIndexBuffer()) {
        RenderCommand::DrawIndexed(vertexArray);
    } else {
        RenderCommand::DrawArrays(vertexArray);
    }
}

void Renderer::SetLights(const std::vector<Ref<Light>>& lights) {
    s_DrawData->Lights = lights;

}

void Renderer::OnWindowResize(int width, int height) {
    RenderCommand::SetViewport(0, 0, width, height);
}

void Renderer::LoadShaderProgram(const std::string& name, const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    Ref<ShaderProgram> shaderProgram = ShaderProgram::Create(name, {
        Shader::Create(vertexShaderPath),
        Shader::Create(fragmentShaderPath),
    });

    s_ShaderProgramLibrary->AddShaderProgram(name, std::move(shaderProgram));
}
Ref<ShaderProgram>& Renderer::GetShaderProgram(const std::string& name) {
    return s_ShaderProgramLibrary->GetShaderProgram(name);
}

}
