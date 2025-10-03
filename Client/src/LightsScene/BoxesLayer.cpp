#include "BoxesLayer.h"
#include "Models/gift.h"

using namespace ZPG;

BoxesLayer::BoxesLayer() {
}
void BoxesLayer::OnAttach() {
    auto basicNormalVertexShader = Shader::Create("./assets/shaders/basic_normal.vert");
    auto basicNormalFragShader = Shader::Create("./assets/shaders/basic_normal.frag");

    std::vector<ZPG::Ref<ZPG::Shader>> shaders;
    shaders.push_back(basicNormalVertexShader);
    shaders.push_back(basicNormalFragShader);

    m_ShaderProgram = ZPG::ShaderProgram::Create("basic_normal.vert.frag", shaders);

    auto VBO = ZPG::VertexBuffer::Create(gift, sizeof(gift)/sizeof(*gift));
    VBO->SetLayout({
        {ZPG::ShaderDataType::Float3, "a_Pos"},
        {ZPG::ShaderDataType::Float3, "a_Normal"},
    });
    
    m_VAO = ZPG::VertexArray::Create();
    m_VAO->AddVertexBuffer(VBO);
}
void BoxesLayer::OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) {

}
void BoxesLayer::OnRender(const ZPG::RenderContext& ctx) {
    ZPG::Renderer::BeginDraw(ctx.m_Camera);
        glm::mat4 modelMat = glm::mat4(1.f);
        Renderer::Submit(m_ShaderProgram, m_VAO, modelMat);
    ZPG::Renderer::EndDraw();
}
