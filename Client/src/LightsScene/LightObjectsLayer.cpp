#include "LightObjectsLayer.h"
#include "Models/gift.h"

using namespace ZPG;

LightObjectsLayer::LightObjectsLayer() {
}
void LightObjectsLayer::OnAttach() {
    auto basicNormalVertexShader = Shader::Create("./assets/shaders/vertex/basic_lighting.vert");
    auto basicNormalFragShader = Shader::Create("./assets/shaders/fragment/phong.frag");

    std::vector<ZPG::Ref<ZPG::Shader>> shaders;
    shaders.push_back(basicNormalVertexShader);
    shaders.push_back(basicNormalFragShader);

    m_ShaderProgram = ZPG::ShaderProgram::Create("basic_lighting+phong", shaders);

    auto VBO = ZPG::VertexBuffer::Create(gift, sizeof(gift)/sizeof(*gift));
    VBO->SetLayout({
        {ZPG::ShaderDataType::Float3, "a_Pos"},
        {ZPG::ShaderDataType::Float3, "a_Normal"},
    });
    
    m_VAO = ZPG::VertexArray::Create();
    m_VAO->AddVertexBuffer(VBO);
}
void LightObjectsLayer::OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) {

}
void LightObjectsLayer::OnRender(const ZPG::RenderContext& ctx) {
    static float rot = 0.f;
    rot += 100 * ctx.m_Timestep;
    auto model = CompoundTransform();
    model.Emplace<RotationTransform>(rot, glm::vec3(0.f, 1.f, 0.f));

    ZPG::Renderer::BeginDraw(ctx.m_Camera);
        Renderer::SetLights(ctx.m_Lights);
        Renderer::Submit(m_ShaderProgram, m_VAO, model.GetMatrix());
    ZPG::Renderer::EndDraw();
}
