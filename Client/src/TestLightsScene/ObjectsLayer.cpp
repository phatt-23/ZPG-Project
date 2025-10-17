#include "ObjectsLayer.h"
#include "../assets/models/nemec/gift.h"

using namespace ZPG;
using namespace glm;

ObjectsLayer::ObjectsLayer() {
}
void ObjectsLayer::OnAttach() {
    auto basicNormalVertexShader = Shader::Create("./assets/shaders/deprecated/basic_lit.vert");
    auto basicNormalFragShader = Shader::Create("./assets/shaders/deprecated/phong_constant_red_color.frag");

    std::vector<ZPG::ref<ZPG::Shader>> shaders;
    shaders.push_back(basicNormalVertexShader);
    shaders.push_back(basicNormalFragShader);

    m_ShaderProgram = ZPG::ShaderProgram::Create("basic_lighting+phong", shaders);

    auto VBO = ZPG::VertexBuffer::Create(nemec::gift, sizeof(nemec::gift));
    VBO->SetLayout({
        {ZPG::ShaderDataType::Float3, "a_Pos"},
        {ZPG::ShaderDataType::Float3, "a_Normal"},
    });
    
    m_VAO = ZPG::VertexArray::Create();
    m_VAO->AddVertexBuffer(VBO);

    m_Transform = MakeRef<TransformGroup>();
    // m_Transform->Push(CreateRef(new TranslationTransform(vec3(0, 0, -2))));
    m_Transform->Include(MakeRef<DynRotate>(0.f, 50.f, v3(0.0, 1.0, 0.0)));
}

void ObjectsLayer::OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) {
    m_Transform->Update(ctx.Ts);
}
void ObjectsLayer::OnRender([[maybe_unused]] const ZPG::RenderContext& ctx) {
    Renderer::Submit(*m_ShaderProgram, *m_VAO, m_Transform->GetMatrix());
}
