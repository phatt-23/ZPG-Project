#include "InlineShadersLayer.h"
#include <imgui.h>
#include "Models/gift.h"

using namespace ZPG;

InlineShadersLayer::InlineShadersLayer() {
}

void InlineShadersLayer::OnAttach() {
    auto basicNormalVertexShader = Shader::CreateFromCode("test.vert", Shader::Vertex, R"(
        #version 330

        layout (location = 0) in vec3 a_Pos;
        layout (location = 1) in vec3 a_Normal;

        uniform mat4 u_Model;
        uniform mat4 u_ViewProj;

        out vec3 v_Normal;

        void main() {
            gl_Position = u_ViewProj * u_Model * vec4(a_Pos, 1.0);
            
            v_Normal = a_Normal;
        }
        )");
    

    auto basicNormalFragShader = Shader::CreateFromCode("basic.frag.inl", Shader::Fragment, R"(
        #version 330

        in vec3 v_Normal;
        out vec4 f_Color;

        void main() {
            f_Color = vec4(0.5f * v_Normal, 1.f);
        }
        )");

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

void InlineShadersLayer::OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) {
}
void InlineShadersLayer::OnRender(const ZPG::RenderContext& ctx) {
    glm::mat4 model = glm::translate(glm::mat4(1.f), glm::vec3(1.0, 0.0, -1.f));
    Renderer::Submit(*m_ShaderProgram, *m_TreeVAO, model);
}
