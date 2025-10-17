#include "ObjectsLayer.h"
#include "../assets/models/nemec/gift.h"

using namespace ZPG;
using namespace glm;

namespace TestModelSceneNS {

ObjectsLayer::ObjectsLayer() {
}
void ObjectsLayer::OnAttach() {
    m_ShaderProgram = ZPG::ShaderProgram::Create("basic_lit+phong", {
        Shader::Create("./assets/shaders/deprecated/basic_lit.vert"),
        Shader::Create("./assets/shaders/deprecated/phong_constant_red_color.frag"),
    });

    auto VBO = ZPG::VertexBuffer::Create(nemec::gift, sizeof(nemec::gift));
    VBO->SetLayout({
        {ZPG::ShaderDataType::Float3, "a_Pos"},
        {ZPG::ShaderDataType::Float3, "a_Normal"},
    });
    
    auto VAO = ZPG::VertexArray::Create();
    VAO->AddVertexBuffer(VBO);

    auto transform = MakeRef<TransformGroup>();
    transform->Include(MakeRef<DynRotate>(0.f, 50.f, v3(0.0, 1.0, 0.0)));

    // ----- model -----
    auto mesh = MakeRef<Mesh>(VAO);
    m_Model = MakeRef(new Model({ mesh }));

    // ----- entity -----------
    auto entityTransform = MakeRef<TransformGroup>();
    entityTransform->Include(MakeRef<Translate>(vec3(0.1, 0.1, 0.1)));
    entityTransform->Include(MakeRef<DynRotate>(0.f, 50.f, v3(0.0, 1.0, 0.0)));

    m_Entity = MakeRef(new Entity(m_Model, entityTransform));
}

void ObjectsLayer::OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) {
    m_Entity->Update(ctx.Ts);
}

void ObjectsLayer::OnRender(const ZPG::RenderContext& ctx) {
    Renderer::SetLights(ctx.Lights);
    Renderer::Submit(*m_ShaderProgram, *m_Entity);
}


}
