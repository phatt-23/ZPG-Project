#include "AxisLayer.h"
#include "Entity/Entity.h"
#include "ZPGine.h"

using namespace ZPG;

namespace AxisSceneNS {

AxisLayer::AxisLayer() {
}

AxisLayer::~AxisLayer() {
}

void AxisLayer::OnAttach() {
    m_ShaderProgram = ShaderProgram::Create("AxisShaderProgram", {
        Shader::Create("./assets/shaders/vertex/DefaultLit.vert"),
        Shader::CreateFromCode("just_a_single_color_thing", Shader::Fragment, R"(
            #version 330 core
            uniform vec3 u_AlbedoColor;
            out vec4 f_FragColor;
            void main() {
                f_FragColor = vec4(u_AlbedoColor, 0.8);
            }
        )"),
    });

    auto VAO = VertexArray::Create({
        VertexBuffer::Create(
            BoxModel::boxVertices, 
            sizeof(BoxModel::boxVertices),
            {
                {ShaderDataType::Float3, "a_Pos"},
                {ShaderDataType::Float3, "a_Normal"},
                {ShaderDataType::Float2, "a_UV"},
            }
        ),
    }, IndexBuffer::Create(BoxModel::boxIndices, ZPG_ARRAY_LENGTH(BoxModel::boxIndices)));

    f32 length = 200.0f;
    auto x = CreateRef<Mesh>(VAO, Scale(glm::vec3(length, 0.1, 0.1)).GetMatrix());
    auto y = CreateRef<Mesh>(VAO, Scale(glm::vec3(0.1, length, 0.1)).GetMatrix());
    auto z = CreateRef<Mesh>(VAO, Scale(glm::vec3(0.1, 0.1, length)).GetMatrix());

    auto xm = CreateRef(new Material());
    xm->SetShaderProgram(m_ShaderProgram);
    xm->SetAlbedoColor(glm::vec3(1.0, 0.0, 0.0));
    x->SetMaterial(xm);

    auto ym = CreateRef(new Material());
    ym->SetShaderProgram(m_ShaderProgram);
    ym->SetAlbedoColor(glm::vec3(0.0, 1.0, 0.0));
    y->SetMaterial(ym);

    auto zm = CreateRef(new Material());
    zm->SetShaderProgram(m_ShaderProgram);
    zm->SetAlbedoColor(glm::vec3(0.0, 0.0, 1.0));
    z->SetMaterial(zm);

    auto model = CreateRef<Model>(new Model({ x, y, z }));

    auto transform = CreateRef<Translate>(glm::vec3(0.0f));

    m_Entity = CreateRef(new Entity(model, transform));
}

void AxisLayer::OnRender([[maybe_unused]] const ZPG::RenderContext& ctx) {
    Renderer::SumbitEntity(*m_Entity);
}

}
