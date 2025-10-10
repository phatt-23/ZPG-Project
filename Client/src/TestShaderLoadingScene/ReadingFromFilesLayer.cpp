#include "ReadingFromFilesLayer.h"
#include <imgui.h>
#include "Models/gift.h"

using namespace ZPG;

ReadingFromFilesLayer::ReadingFromFilesLayer() {
}

void ReadingFromFilesLayer::OnAttach() {
    auto basicNormalVertexShader = ZPG::Shader::Create("./assets/shaders/basic_normal.vert");
    auto basicNormalFragShader = ZPG::Shader::Create("basic.frag", "./assets/shaders/basic_normal.frag");

    std::vector<ZPG::Ref<ZPG::Shader>> shaders;
    shaders.push_back(basicNormalVertexShader);
    shaders.push_back(basicNormalFragShader);

    m_ShaderProgram = ZPG::ShaderProgram::Create("basic_normal.vert.frag", shaders);

    auto treeVBO = ZPG::VertexBuffer::Create(gift, sizeof(gift));
    treeVBO->SetLayout({
        {ZPG::ShaderDataType::Float3, "a_Pos"},
        {ZPG::ShaderDataType::Float3, "a_Normal"},
    });
    
    m_TreeVAO = ZPG::VertexArray::Create();
    m_TreeVAO->AddVertexBuffer(treeVBO);
    
}

void ReadingFromFilesLayer::OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) {
}
void ReadingFromFilesLayer::OnRender([[maybe_unused]] const ZPG::RenderContext& ctx) {
    Renderer::Submit(*m_ShaderProgram, *m_TreeVAO, glm::mat4(1.f));
}
