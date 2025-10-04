#include "TreeLayer.h"
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "imgui.h"
#include "Models/tree.h"
#include "Models/suzi_smooth.h"
#include "Models/suzi_flat.h"
#include "Models/sphere.h"
#include "Models/plain.h"
#include "Models/bushes.h"
#include "Models/gift.h"

using namespace ZPG;

static f32 planeVertices[] = {
    -0.1f, -0.5f, 0.1f,    1.0, 1.0, 1.0, 1.0,
    0.1f, -0.5f, 0.1f,    1.0, 1.0, 1.0, 1.0,
    0.1f, -0.5f, -0.1f,    1.0, 1.0, 1.0, 1.0,
    -0.1f, -0.5f, -0.1f,    1.0, 1.0, 1.0, 1.0,
};

static u32 planeIndices[] = {
    0, 1, 2,
    0, 2, 3,
};

TreeLayer::TreeLayer() {
}
void TreeLayer::OnAttach() {
    m_NormalShaderProgram = ShaderProgram::Create("./assets/shaders/basic_normal.glsl");
    m_RedShaderProgram = ShaderProgram::Create("./assets/shaders/red.glsl");

    auto treeVBO = VertexBuffer::Create(tree, ZPG_ARRAYSIZE(tree));
    treeVBO->SetLayout({
        {ShaderDataType::Float3, "a_Pos"},
        {ShaderDataType::Float3, "a_Normal"},
    });

    m_TreeVAO = VertexArray::Create();
    m_TreeVAO->AddVertexBuffer(treeVBO);
    
    auto planeVbo = VertexBuffer::Create(planeVertices, ZPG_ARRAYSIZE(planeVertices));
    planeVbo->SetLayout({
        {ShaderDataType::Float3, "a_Pos"},
        {ShaderDataType::Float4, "a_Color"},
    });

    auto planeIbo = IndexBuffer::Create(planeIndices, ZPG_ARRAYSIZE(planeIndices));
    m_PlaneVao = VertexArray::Create();
    m_PlaneVao->AddVertexBuffer(planeVbo);
    m_PlaneVao->SetIndexBuffer(planeIbo);
}
void TreeLayer::OnUpdate([[maybe_unused]] SceneContext& context) {
}
void TreeLayer::OnRender([[maybe_unused]] const ZPG::RenderContext& ctx) {
    static f32 rot = 0;
    rot += 50.f * ctx.m_Timestep;
    static glm::mat4 treeTransform = glm::mat4(1.f);
    treeTransform = glm::rotate(glm::mat4(1.f), glm::radians(rot), glm::vec3(0.f, 1.f, 0.f));

    static glm::mat4 planeTransform = glm::scale(glm::mat4(1.f), glm::vec3(10.f, 1.0f, 20.f));
    Renderer::BeginDraw(ctx.m_Camera);
        Renderer::Submit(m_NormalShaderProgram, m_TreeVAO, treeTransform);
        Renderer::Submit(m_RedShaderProgram, m_PlaneVao, planeTransform);
    Renderer::EndDraw();
}

