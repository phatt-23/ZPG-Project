#include "ObjectsLayer.h"
#include "../assets/models/nemec/gift.h"

using namespace ZPG;
using namespace glm;

namespace TestMeshSceneNS {

ObjectsLayer::ObjectsLayer() {
}
void ObjectsLayer::OnAttach() {
    auto vertexShader = Shader::Create("./assets/shaders/deprecated/basic_lit.vert");
    auto fragmentShader = Shader::Create("./assets/shaders/deprecated/phong_constant_red_color.frag");

    m_ShaderProgram = ZPG::ShaderProgram::Create("basic_lighting+phong", {
        vertexShader,
        fragmentShader,
    });

    auto VBO = ZPG::VertexBuffer::Create(nemec::gift, sizeof(nemec::gift));
    VBO->SetLayout({
        {ZPG::ShaderDataType::Float3, "a_Pos"},
        {ZPG::ShaderDataType::Float3, "a_Normal"},
    });
    
    m_VAO = ZPG::VertexArray::Create();
    m_VAO->AddVertexBuffer(VBO);

    m_Transform = MakeRef<TransformGroup>();
    // m_Transform->Push(CreateRef(new Transform(scale(mat4(1.0f), vec3(2.0, 3.0, 4.0)))));
    // m_Transform->Push(CreateRef(new RotationTransform(45.f, vec3(0, 1, 0))));
    // m_Transform->Push(CreateRef(new ScaleTransform(vec3(2, 3, 5))));
    m_Transform->Include(MakeRef(new Translate(vec3(0, 0, -2))));
    // m_Transform->Push(CreateRef(new DynRotationTransform(
    //                                     0.f, 
    //                                     50.f, 
    //                                     glm::vec3(0.0, 1.0, 0.0))));
    // m_Transform->Push(CreateRef(new DynTranslationTransform(
    //                                     vec3(0.0, 0.0, 0.0),
    //                                     vec3(0.1, 0.1, 0.1),
    //                                     vec3(0.0, 0.0, 0.0),
    //                                     vec3(1.0, 1.0, 1.0))));
    // m_Transform->Push(CreateRef(new DynScaleTransform(
    //                                     vec3(1.0, 1.0, 1.0),
    //                                     2.0f * vec3(0.2, 0.4, 0.8),
    //                                     vec3(0.1, 0.1, 0.1),
    //                                     vec3(5.0, 5.0, 5.0))));
    // m_Transform->Push(CreateRef(new PulseScaleTransform(
    //                                     vec3(1.0, 1.0, 1.0),
    //                                     5.f * vec3(0.2, 0.4, 0.8),
    //                                     vec3(1, 1, 1),
    //                                     2.f * vec3(1, 1, 1))));

    // ----- mesh -----------
    Translate trTrans(vec3(0.2, 0.0, -1.0));
    m_Mesh = MakeRef(new Mesh(m_VAO, trTrans.GetMatrix()));


    // ----- entity -----------
    auto entityMesh = MakeRef(new Mesh(m_VAO));
    
    auto entityTransform = MakeRef(new TransformGroup());
    entityTransform->Include(MakeRef(new Translate(vec3(0.1, 0.1, 0.1))));
    entityTransform->Include(MakeRef(new DynRotate(0.f, 50.f, glm::vec3(0.0, 1.0, 0.0))));
}

void ObjectsLayer::OnUpdate([[maybe_unused]] ZPG::SceneContext& ctx) {
    m_Transform->Update(ctx.Ts);
}

void ObjectsLayer::OnRender(const ZPG::RenderContext& ctx) {
    ZPG::Renderer::BeginDraw(ctx.Cam);
        Renderer::SetLights(ctx.Lights);
        Renderer::Submit(*m_ShaderProgram, *m_VAO, m_Transform->GetMatrix());
        // Renderer::Submit(m_ShaderProgram, m_Mesh->GetVertexArray(), m_Mesh->GetLocalTransform());
        Renderer::Submit(*m_ShaderProgram, *m_Mesh);
        Renderer::Submit(*m_ShaderProgram, *m_Mesh, Translate(vec3(-0.4, 0.2, 0.0)).GetMatrix());
    ZPG::Renderer::EndDraw();
}


}
