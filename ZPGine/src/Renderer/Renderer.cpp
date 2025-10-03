#include "Renderer.h"
#include "RenderCommand.h"

namespace ZPG {

void Renderer::Init() {
    RenderCommand::Init();
}
void Renderer::Shutdown() {
    RenderCommand::Shutdown();
}
void Renderer::BeginDraw(const Camera& camera) {
    s_DrawData->ViewProjMatrix = camera.GetViewProjMatrix();
}
void Renderer::EndDraw() {
    // nothing for now
    // batch the drawing by instances of the same DrawableObject, shader being used and material
    // setup the submitted lights into the shaders
    s_DrawData->Lights.clear();
}
void Renderer::Submit(const Ref<ShaderProgram>& shaderProgram, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
    shaderProgram->Bind();
    shaderProgram->SetMat4("u_Model", transform);
    shaderProgram->SetMat4("u_ViewProj", s_DrawData->ViewProjMatrix);

    auto& lights = s_DrawData->Lights;
    size_t index = 0;
    for (index = 0; index < lights.size(); index++) {
        if (lights[index] == nullptr) 
            continue;
        lights[index]->SendToShaderProgram(*shaderProgram, index);
    }
    shaderProgram->SetInt("u_LightCount", index);

    vertexArray->Bind();

    if (vertexArray->HasIndexBuffer()) {
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

}
