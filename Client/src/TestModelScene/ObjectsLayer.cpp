#include "ObjectsLayer.h"
#include "Models/gift.h"

using namespace ZPG;
using namespace glm;

namespace TestModelSceneNS {

ObjectsLayer::ObjectsLayer() {
}
void ObjectsLayer::OnAttach() {
    m_ShaderProgram = ZPG::ShaderProgram::Create("basic_lit+phong", {
        Shader::Create("./assets/shaders/vertex/basic_lit.vert"),
        Shader::Create("./assets/shaders/fragment/phong_constant_red_color.frag"),
    });

    auto VBO = ZPG::VertexBuffer::Create(gift, sizeof(gift));
    VBO->SetLayout({
        {ZPG::ShaderDataType::Float3, "a_Pos"},
        {ZPG::ShaderDataType::Float3, "a_Normal"},
    });
    
    auto VAO = ZPG::VertexArray::Create();
    VAO->AddVertexBuffer(VBO);

    auto transform = CreateRef<CompoundTransform>();
    transform->Push(CreateRef(new DynRotationTransform(
                                        0.f, 
                                        50.f, 
                                        glm::vec3(0.0, 1.0, 0.0))));

    // ----- model -----
    auto mesh = CreateRef<Mesh>(VAO);
    m_Model = CreateRef(new Model({ mesh }));

    // ----- entity -----------
    auto entityTransform = CreateRef<CompoundTransform>();
    entityTransform->Push(CreateRef<TranslationTransform>(vec3(0.1, 0.1, 0.1)));
    entityTransform->Push(CreateRef<DynRotationTransform>(0.f, 50.f, glm::vec3(0.0, 1.0, 0.0)));

    m_Entity = CreateRef(new Entity(m_Model, entityTransform));
}

void ObjectsLayer::OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) {
    m_Entity->Update(ctx.m_Timestep);
}

void ObjectsLayer::OnRender(const ZPG::RenderContext& ctx) {
    Renderer::SetLights(ctx.m_Lights);
    Renderer::Submit(*m_ShaderProgram, *m_Entity);
}


}
